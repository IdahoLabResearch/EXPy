# Vendored libcbv2g patch: ISO20 PGPData union-hoist + grammar rewrite (#25)

ADR-0007 and ADR-0008 fixed the ISO-2 PGPData grammar. The iso20 family (`iso20_CommonMessages`, `iso20_AC`, `iso20_DC`, `iso20_WPT`, `iso20_ACDP`) has the same `iso20_*_PGPDataType` struct shape — `choice_N_isUsed` flags sharing union storage with the `choice_N` substructs — and the same data-aliasing failure mode ADR-0007 describes for ISO-2. ADR-0007 noted in passing that "the iso20 namespaces do not have the same union-aliased dispatch pattern"; that footnote is incorrect and is corrected by this ADR. The two scenarios `KeyInfo__choice_PGPData` and `PGPData__maximal` were strict-`xfail` against #25 in `tests/integration/test_iso20_*_generated_roundtrip.py` (and the matching `*_xmldsig_roundtrip.py`) until this patch landed.

Beyond the union aliasing, the iso20 PGPData grammar tables emitted by upstream cbexigen are independently broken in ways the ISO-2 tables are not. Four distinct defects combined to make seq-1's optional particles, seq-2's entry, and seq-2's tail unreachable or non-terminating:

1. **Encoder grammar `G0` (entry, e.g. 37 in Common / 35 in AC/DC/ACDP / 40 in WPT).** The seq-2 dispatch was implicit `else` of `choice_1_isUsed`. With both `choice_*_isUsed` clobbered by union aliasing, the encoder fell through to the seq-2 branch unconditionally — then read PGPKeyPacket from `choice_1.PGPKeyPacket` instead of `choice_2.PGPKeyPacket`, and transitioned to grammar `G2` (the seq-1 after-PGPKeyPacket state) instead of grammar `G4` (the seq-2 after-PGPKeyPacket state).
2. **Encoder grammar `G1` (seq-1 after PGPKeyID).** The first branch gated PGPKeyPacket emission on `choice_1_isUsed` (always 1 in seq-1, so PGPKeyPacket was emitted even when absent). The second branch was an `else if (choice_1_isUsed == 1u)` — identical predicate to the first branch, so the ANY-emission code was dead. A seq-1 minimal (PGPKeyID only) therefore emitted phantom empty PGPKeyPacket bytes, and a seq-1 fixture with ANY-but-no-PGPKeyPacket could never emit its ANY.
3. **Encoder grammar `G2` (seq-1 after PGPKeyPacket) and `G1` ANY branch.** The ANY-emission transition pointed to grammar `G3`, but grammar `G3` was the seq-2 first-PGPKeyPacket emitter — re-emitting `choice_2.PGPKeyPacket` after the seq-1 ANY's inner END Element bit, which is not a valid continuation in seq-1. Worse, grammar `G3`'s transition to `G4` (seq-2 after-PGPKeyPacket) and `G4`'s ANY loop back to `G3` formed an infinite encoder loop until the bit-stream buffer overflowed.
4. **Encoder grammar `G4` (seq-2 after PGPKeyPacket).** The ANY branch was gated on `choice_2_isUsed` (always 1 in seq-2, so ANY was emitted even when absent) and transitioned back to `G3` (re-emit PGPKeyPacket) instead of terminating.

The decoders mirror the same defects: grammar `G0` case 0 (seq-1 PGPKeyID entry) never set the outer `choice_1_isUsed`, so `getJson_PGPDataType` always returned `{}` even when the encoder did emit a complete seq-1 payload. Grammar `G0` case 1 (PGPKeyPacket-as-first-element) wrote into `choice_1.PGPKeyPacket` and set `choice_1.PGPKeyPacket_isUsed = 1u` — both wrong; the byte pattern is only emitted as the seq-2 entry. Grammars `G1`/`G2` case 3 and grammar `G4` case 2 transitioned to grammar `G3` on consumption of the ANY event, mirroring the encoder's loop.

## Patch

The fix lives in EXPy as two patches under `patches/libcbv2g/`, applied alongside the ADR-0007/0008/0009 patches by the `libcbv2g` Make target. The auto-discovery wildcard in the Makefile (`LIBCBV2G_PATCHES = $(sort $(wildcard patches/libcbv2g/*.patch))`) picks the new patches up without configuration changes. The submodule remains pristine at its pinned upstream commit; the apply step probes with `git apply --check --reverse` first, so re-running `make` after the patches are in place prints `skip … (already applied)`. `make clean` deletes only `extern/libcbv2g/build/` and leaves the patched working tree intact; `git -C extern/libcbv2g checkout -- .` restores the submodule to upstream if needed.

Patch 0004 (`patches/libcbv2g/0004-iso20-pgpdata.patch`) hoists the `choice_N_isUsed` flags out of the anonymous union in all five iso20 namespace headers. The `choice_1` and `choice_2` substructs continue to share storage (one is active at a time); only the dispatch flags are moved out. Five files are modified:

- `include/cbv2g/iso_20/iso20_CommonMessages_Datatypes.h`
- `include/cbv2g/iso_20/iso20_AC_Datatypes.h`
- `include/cbv2g/iso_20/iso20_DC_Datatypes.h`
- `include/cbv2g/iso_20/iso20_WPT_Datatypes.h`
- `include/cbv2g/iso_20/iso20_ACDP_Datatypes.h`

Patch 0005 (`patches/libcbv2g/0005-iso20-pgpdata-grammar.patch`) rewrites the encoder/decoder grammars for all five namespaces. The same shape of transformation is applied across the five namespaces, with grammar-id offsets per namespace (Common: 37–41; AC/DC/ACDP: 35–39; WPT: 40–44). For each namespace `NS` with entry grammar `G0` and four subsequent grammars `G1..G4`:

- **Encoder `G0` else-branch**: dispatch on `choice_2_isUsed`, read from `choice_2.PGPKeyPacket`, transition to `G4`.
- **Encoder `G1` first branch**: gate on `choice_1.PGPKeyPacket_isUsed` (was `choice_1_isUsed`).
- **Encoder `G1` second branch**: gate on `choice_1.ANY_isUsed` (was the dead duplicate `choice_1_isUsed`), and complete the element after the inner ANY END bit (`done = 1; grammar_id = 3;`) instead of transitioning to `G3`.
- **Encoder `G2` first branch**: gate on `choice_1.ANY_isUsed` and complete the element after ANY.
- **Encoder `G3`**: body replaced with `error = EXI_ERROR__UNKNOWN_GRAMMAR_ID;` — unreachable in the patched state machine, kept as a defensive trap.
- **Encoder `G4` first branch**: gate on `choice_2.ANY_isUsed` and complete the element after ANY.
- **Decoder `G0` case 0**: set `PGPDataType->choice_1_isUsed = 1u` before the grammar transition.
- **Decoder `G0` case 1**: write into `choice_2.PGPKeyPacket`, set `PGPDataType->choice_2_isUsed = 1u`, transition to `G4`.
- **Decoder `G1`/`G2` case 3** and **`G4` case 2**: after consuming the ANY value, set `done = 1; grammar_id = 3;` instead of transitioning to `G3`.

Each patched site carries an inline `EXPy patch (ADR-0010): …` comment, mirroring the earlier ADRs. A grep for `EXPy patch` against an applied submodule tree surfaces every divergence across all four ADRs (0007/0008/0009/0010).

Grammar `G3` is now unreachable in the patched encoder state machine. Leaving the case label with an error return (rather than deleting it) minimises structural churn against the upstream switch table and fails loudly if a future code path reaches it by accident.

The bit-stream layout is unchanged at every emission point — only the conditions guarding emission, the destination grammar IDs, and the substruct field references change. The event codes (2-bit at `G0`, 3-bit at `G1`/`G2`, 1-bit at `G3`, 2-bit at `G4`) and the per-event sub-bits and value encodings are byte-for-byte identical to the upstream tables.

## Choice manifest entries

After the patches land, the per-namespace choice manifests in `tools/codegen/iso20_*_choices.py` gain a `*_PGPDataType: [[["choice_1"], ["choice_2"]]]` entry. This replaces each namespace's `PGPData__minimal` / `PGPData__maximal` scenarios with `PGPData__choice_choice_1` / `PGPData__choice_choice_2`, and also forces the inner PGPData inside the `KeyInfo__choice_PGPData` scenario to default to branch 0 (seq-1) — without the manifest entry, the fixture generator emits both `choice_1` and `choice_2` keys for the nested PGPDataType, and the encoder honours only one branch per choice. The xfail blocks for these two scenarios were removed from the eight affected test files.

## Scope

The patch covers all five iso20 namespaces. The DIN schema does not declare a PGPDataType, and ISO-2's PGPDataType is covered by ADR-0007 and ADR-0008. Inside each iso20 namespace, the patch covers exactly the PGPDataType encoder/decoder pair — no other types are touched. The fixture-emission and roundtrip coverage now includes both choice branches for every iso20 namespace; the `KeyInfo__choice_PGPData` scenario reaches the seq-1 branch indirectly via the manifest's default-branch-0 selection for nested choices.

## Considered Options

- **Fix only patch 0004 (hoist flags) and not patch 0005 (grammar rewrite).** Rejected. ADR-0007's analogous union-hoist alone was sufficient for ISO-2 because ISO-2's grammar tables were structurally correct outside the data-aliasing issue. The iso20 grammar tables have independent structural bugs (dead-code branches, wrong substruct reads, missing END transitions, infinite loops) — hoisting alone leaves seq-1 minimal failing because of grammar 38's dead ANY branch, seq-2 entry failing because of grammar 37's wrong substruct, and seq-2 with ANY failing because of grammar 41's loop back to grammar 40.
- **Combine 0004 and 0005 into a single patch.** Rejected. The two patches modify disjoint file sets — 0004 touches only the five `*_Datatypes.h` headers, 0005 touches only the ten encoder/decoder `.c` files — and have independent failure modes (struct ABI vs. EXI bit stream). Keeping them separate matches the ADR-0007/0008 lineage shape and makes a re-pin failure (`git apply` mismatch on one patch) localise the diagnosis to either the header or the codec.
- **Regenerate libcbv2g from its own schema.** Rejected for the same reasons as in ADR-0007 and ADR-0008. The upstream code generator emits both the broken struct layout and the broken grammar tables; touching the generator is a much larger change that needs to be upstreamed. The hand-patch is a surgical local fix until that work happens.
- **Patch only iso20_Common and let AC/DC/WPT/ACDP fail.** Rejected. The xmldsig types are W3C-standard and are duplicated by the upstream generator into each iso20 namespace's header with the namespace-specific prefix; the bug is identical in all five. The xfail markers existed in all eight test files (`{Common,AC,DC,WPT,ACDP}_generated_roundtrip.py` plus `{Common,AC,DC}_generated_xmldsig_roundtrip.py`); fixing only Common would leave seven of those eight as still-broken acceptance-criteria-misses. The patch is structurally identical across the five namespaces — only grammar-id offsets and type prefixes change — so the cost of covering all five is minimal once the Common version is in place.
- **Drop grammar `G3` (the unreachable seq-2 PGPKeyPacket emitter).** Rejected. Removing the `case` label means the switch defaults to `EXI_ERROR__UNKNOWN_GRAMMAR_ID` if any path reaches it, which is the same observable behaviour as the explicit error-return body. Keeping the explicit `case` label keeps the switch table visually parallel to the upstream output and surfaces in `grep` for `case 39`/`case 37`/`case 44` (per-namespace grammar IDs) when reading the patched tree.

## Consequences

- The 16 strict-`xfail` markers across the eight iso20 `*_generated_roundtrip.py` and `*_generated_xmldsig_roundtrip.py` files are removed; each file gains two new passing scenarios (`PGPData__choice_choice_1`, `PGPData__choice_choice_2`) and the `KeyInfo__choice_PGPData` scenario moves from xfail to passing. The PGPData-related `_NESTED_CHOICE_XFAILS` blocks and `_param` wrappers in those files are deleted; one file (`test_iso20_wpt_generated_roundtrip.py`) retains a `_LOOP_GRAMMAR_XFAILS` block for ADR-0009-tracked scenarios, with the PGPData entry stripped.
- ADR-0007's footnote ("the iso20 namespaces do not have the same union-aliased dispatch pattern") is corrected — see the inline note at the bottom of ADR-0007.
- Any future re-pin of `extern/libcbv2g/` must keep `patches/libcbv2g/0004-iso20-pgpdata.patch` and `patches/libcbv2g/0005-iso20-pgpdata-grammar.patch` applying cleanly, or every iso20 namespace's PGPData round-trip silently regresses. The Make target prints `skip` when each patch is already applied and aborts with a git-apply error if a patch no longer matches the new upstream, so a re-pin that breaks either fails the build instead of silently regressing.
- ADR-0001 ("codegen from libcbv2g headers") still holds: the EXPy-side marshalers are still generated from the vendored headers. Patch 0004 shifts the struct layout the marshalers see (the marshaler reads `outDoc.choice_*_isUsed` as plain field accesses, which works correctly once the flags have their own storage); patch 0005 does not change the headers and therefore does not affect codegen.
