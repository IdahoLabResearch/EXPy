# Vendored libcbv2g patch: hoist PGPData choice flags out of union

The libcbv2g code generator emitted `struct iso2_PGPDataType` (in `extern/libcbv2g/include/cbv2g/iso_2/iso2_msgDefDatatypes.h`) with `choice_1_isUsed` and `choice_2_isUsed` declared as members of the same anonymous union as the `choice_1` and `choice_2` substructs. Because all union members share storage, the dispatch flags physically aliased the substruct data: the marshaler `getDoc_PGPDataType` would set `choice_1_isUsed = 1` and then have the bit clobbered by the immediately following data write into `choice_1.PGPKeyID.bytes[0]`. The encoder would then read `choice_1_isUsed` as whatever LSB the data happened to carry (zero for the all-zeros fixture seed), pick the wrong grammar branch, and emit nothing for the PGPData element. Symmetrically, the decoder had a related defect: even when seq-1 grammar was followed, it never set the outer `choice_N_isUsed` flags, so the marshaler `getJson_PGPDataType` would see them as zero and produce an empty `{}`. The two scenarios `PGPData__maximal` and `KeyInfo__choice_PGPData` (now `PGPData__choice_choice_1` after the manifest entry in `tools/codegen/iso2_choices.py`) failed for both reasons. This ADR records the patch we applied to the vendored upstream tree so future re-syncs preserve it.

## Patch

The fix lives in EXPy, not in the libcbv2g submodule. We keep `extern/libcbv2g/` pristine at its pinned upstream commit; the patch is shipped as `patches/libcbv2g/0001-iso2-pgpdata.patch` and applied by the `libcbv2g` Make target with `git apply` before cmake runs. The apply step probes with `git apply --check --reverse` first so re-running `make` is a no-op once the patch is in place (`make` prints `skip … (already applied)`). A `make clean` deletes `extern/libcbv2g/build/` but does not touch the working tree; if you ever need to restore the submodule to upstream state, `git -C extern/libcbv2g checkout -- .` is enough.

Two files inside the submodule's working tree are modified by the patch:

- `include/cbv2g/iso_2/iso2_msgDefDatatypes.h`: `choice_1_isUsed` and `choice_2_isUsed` are declared as siblings of the anonymous union, not as members of it. The `choice_1` and `choice_2` substructs continue to share storage (one is active at a time); only the dispatch flags are moved out.
- `lib/cbv2g/iso_2/iso2_msgDefDecoder.c`: in `decode_iso2_PGPDataType`, grammar 65 case 0 (the PGPKeyID-first path, which uniquely identifies sequence 1) now sets `PGPDataType->choice_1_isUsed = 1u` on success. No other decoder positions are touched.

The encoder in `lib/cbv2g/iso_2/iso2_msgDefEncoder.c` is *not* touched: it already reads `PGPDataType->choice_1_isUsed` as a plain field access, which works correctly once the flag has its own storage.

The marshaler in `src/generated/ISO2_marshalers.generated.cpp` is *not* touched: it already sets `outDoc.choice_1_isUsed = 1` and `outDoc.choice_2_isUsed = 1` as plain field writes; before the patch those writes were silently clobbered by the following data writes into the union, and the patch makes them survive.

Each patched site carries an inline comment `EXPy patch (ADR-0007): …` so a grep for `EXPy patch` against an applied submodule tree surfaces every divergence.

## Scope

The patch lands the sequence-1 path of `PGPDataType`. The sequence-2 path remains broken upstream for a different reason — the encoder at grammar 65's else-branch reads from `choice_1.PGPKeyPacket` regardless of which substruct the caller populated, and grammar transitions 65→67 collapse sequence 2 into a sequence-1-without-PGPKeyID shape. Fixing that requires rewriting the libcbv2g grammar transitions, not just moving fields. The corresponding scenario `PGPData__choice_choice_2` is marked strict-`xfail` in `tests/integration/test_iso2_generated_xmldsig_roundtrip.py` with a reason pointing at this ADR, and a follow-up issue tracks the deeper grammar work.

The patch is iso2-only. The DIN schema does not declare a PGPDataType, and the iso20 namespaces do not have the same union-aliased dispatch pattern.

## Considered Options

- **Python-side / V2Gjson workaround.** Rejected. The flags physically alias the substruct data in the compiled C struct, so no ordering of writes from Python can produce both a valid `_isUsed=1` and intact data. Any fix has to be in the C struct layout itself.
- **Commit the patch directly to the submodule checkout.** Rejected. Keeping the submodule pristine at its pinned upstream commit lets `git submodule update --init` always produce the same starting state, makes the project self-contained without forking libcbv2g, and keeps the *delta* (a 52-line patch) trivially reviewable in the EXPy tree.
- **Override copies of the patched files inside EXPy.** Rejected. Two override files (~1KB each) would duplicate enough surrounding code that the delta isn't visible without a manual diff against upstream, and any future libcbv2g re-pin would silently keep using the stale overrides.
- **Regenerate libcbv2g from its own schema.** Rejected for this push. The upstream code generator emits this layout pattern across the iso2 schema (the X509Data type has a similar union, though without the alias-data hazard because its choice_N substructs are flat enough that the flag bit doesn't fall on a data byte that matters). Touching the generator is a much larger change and would have to be upstreamed. The hand-patch is a surgical local fix until that work happens.
- **Patch the C struct only, not the decoder.** Rejected. The decoder never sets the outer `choice_N_isUsed` flags upstream, so getJson_PGPDataType would still return `{}` after a successful encode/decode. Both files have to move together.

## Consequences

- The `PGPDataType` entry is restored to `tools/codegen/iso2_choices.py`, which means the xmldsig roundtrip suite emits per-branch scenarios (`PGPData__choice_choice_1` and `PGPData__choice_choice_2`). The choice_1 scenario and `KeyInfo__choice_PGPData` (which reaches PGPDataType with the choice-1 default) now pass. The choice_2 scenario stays strict-`xfail` pending the follow-up.
- Any future re-pin of the `extern/libcbv2g/` submodule must keep `patches/libcbv2g/0001-iso2-pgpdata.patch` applying cleanly, or the choice_1 path will silently regress to returning `{}`. The Make target prints `skip` when the patch is already applied and aborts with a git-apply error if the patch no longer applies to the new upstream, so a re-pin that breaks the patch fails the build instead of silently regressing.
- ADR-0001 ("codegen from libcbv2g headers") still holds: the EXPy-side marshalers are still generated from the vendored header. The patch shifts the layout the marshalers see, but does not change the codegen pipeline or its inputs.
