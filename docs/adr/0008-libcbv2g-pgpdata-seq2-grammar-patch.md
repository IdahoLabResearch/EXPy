# Vendored libcbv2g patch: PGPData sequence-2 grammar (#22)

ADR-0007 fixed the sequence-1 path of `iso2_PGPDataType` by hoisting the `choice_N_isUsed` dispatch flags out of the anonymous union and teaching the decoder to set `choice_1_isUsed` on the seq-1 entry event. The sequence-2 path remained broken upstream for separate reasons in the EXI grammar tables, captured here. The corresponding scenario `PGPData__choice_choice_2` in `tests/integration/test_iso2_generated_xmldsig_roundtrip.py` was strict-`xfail` against #22 until this patch landed.

Three distinct upstream bugs in libcbv2g's iso2 PGPDataType grammar combined to make the sequence-2 path unreachable:

1. **Encoder grammar 65 else-branch (`lib/cbv2g/iso_2/iso2_msgDefEncoder.c` ~line 3279).** When `choice_1_isUsed == 0` (i.e., the caller selected seq 2), the encoder emitted the first PGPKeyPacket by reading from `PGPDataType->choice_1.PGPKeyPacket` rather than `choice_2.PGPKeyPacket`. With the union hoisted by ADR-0007 the choice_1 substruct is still zero-initialised in seq-2 calls, so the encoder wrote an empty PGPKeyPacket. The same branch also transitioned to grammar 67 (the "after PGPKeyPacket in seq 1" state), so the seq-2 continuation states (68/69) were never reached from the entry event.
2. **Decoder grammar 65 case 1 (`lib/cbv2g/iso_2/iso2_msgDefDecoder.c` ~line 3925).** Symmetric to the encoder: the decoder wrote the seq-2 entry's PGPKeyPacket into `choice_1.PGPKeyPacket`, set `choice_1.PGPKeyPacket_isUsed = 1u`, and transitioned to grammar 67. The outer `choice_2_isUsed` flag was never set, so `getJson_PGPDataType` always saw the result as choice-1-with-no-PGPKeyID and returned `{}`.
3. **Grammar 69's continuation after the seq-2 ANY (encoder and decoder).** Grammar 69 represents "after PGPKeyPacket in seq 2" and accepts the optional `ANY` followed by `END`. The encoder gated emission of the ANY event on `choice_2_isUsed` (which is *always* 1 in a seq-2 call) instead of on `choice_2.ANY_isUsed`, so it tried to emit a zero-length ANY any time seq 2 was selected. Both encoder and decoder then transitioned to grammar 68 (the "START PGPKeyPacket" state) after consuming ANY, despite ANY being seq 2's last optional particle — turning the encoder into an infinite emitter loop until the stream buffer overflowed and aborting the decoder once the post-ANY bits were exhausted.

## Patch

The fix lives in EXPy at `patches/libcbv2g/0002-iso2-pgpdata-seq2.patch`, applied alongside `0001-iso2-pgpdata.patch` by the `libcbv2g` Make target. The submodule remains pristine at its pinned upstream commit; the apply step probes with `git apply --check --reverse` first, so re-running `make` after the patches are in place prints `skip … (already applied)`. `make clean` deletes only `extern/libcbv2g/build/` and leaves the patched working tree intact; `git -C extern/libcbv2g checkout -- .` restores the submodule to upstream if needed.

Two files inside the submodule's working tree are modified by patch 0002:

- `lib/cbv2g/iso_2/iso2_msgDefEncoder.c`: the grammar 65 else-branch now reads from `choice_2.PGPKeyPacket` and transitions to grammar 69 after emitting the seq-2 entry. Grammar 69's ANY-emission branch is gated on `choice_2.ANY_isUsed` and transitions to `done = 1; grammar_id = 4;` after the END marker bit instead of looping back through grammar 68.
- `lib/cbv2g/iso_2/iso2_msgDefDecoder.c`: grammar 65 case 1 (PGPKeyPacket-as-first-element) writes into `choice_2.PGPKeyPacket`, sets `PGPDataType->choice_2_isUsed = 1u`, and transitions to grammar 69. Grammar 69 case 2 (consume seq-2 ANY) completes the element via `done = 1; grammar_id = 4;` instead of falling through to grammar 68.

Each patched site carries an inline `EXPy patch (ADR-0008): …` comment, mirroring ADR-0007. A grep for `EXPy patch` against an applied submodule tree surfaces every divergence (both ADR-0007 and ADR-0008 sites).

The `iso2_PGPDataType` struct layout from ADR-0007 is unchanged: `choice_1_isUsed` and `choice_2_isUsed` remain hoisted out of the anonymous union. Grammar 68 itself is not touched — under the patched transitions it becomes unreachable, but leaving it in place is closer to the upstream output and avoids gratuitous structural churn.

## Scope

The patch is iso2-only and seq-2-specific. ADR-0007's choice_1 path still passes (`KeyInfo__choice_PGPData`, `PGPData__choice_choice_1`); both choice paths share the union storage and the hoisted-out flags from patch 0001. The scenario manifest in `tools/codegen/iso2_choices.py` already emits `PGPData__choice_choice_2` as the maximal seq-2 fixture — with both `PGPKeyPacket` and the optional `ANY` populated — so the same scenario locks both the 65→69 entry transition and the 69-terminates-after-ANY transition.

The DIN schema does not declare a PGPDataType, and the iso20 namespaces do not have the same union-aliased dispatch pattern.

## Considered Options

- **Route grammar 65 case 1 through grammar 68 (the "START PGPKeyPacket" state) instead of inlining the payload.** Rejected. Grammar 68 emits its own 1-bit event code before the PGPKeyPacket payload, so routing through 68 would change the produced bit stream compared to the symmetric seq-1 entry at grammar 65 case 0 (which emits its PGPKeyID inline). The current patch keeps 65 case 1 structurally symmetric to 65 case 0.
- **Fix only the field references and leave the transitions pointing at grammar 67 / 68.** Rejected. Even with the right substruct, the seq-2 maximal scenario (the only one the fixture generator emits for choice_2) populates both PGPKeyPacket and ANY; sending it through grammar 67 collapses seq 2 into a seq-1-without-PGPKeyID shape, and grammar 69's `→ 68` transition after ANY produces the infinite encoder loop described above. Both transitions have to move together to make the maximal case work.
- **Regenerate libcbv2g from its own schema.** Rejected for the same reasons as in ADR-0007. The upstream code generator emits the broken grammar tables; touching the generator is a much larger change that needs to be upstreamed. The hand-patch is a surgical local fix until that work happens.
- **Drop the maximal-with-ANY scenario and only assert the minimal seq-2 path.** Rejected. The acceptance criteria for #22 specifically call out covering choice_2 with its optional `ANY` field populated, which is what exercises the grammar 68/69 transitions. Without the maximal scenario, the 65→69 entry fix would silently regress the next time someone runs the seq-2 path with both fields set.

## Consequences

- The strict-`xfail` in `tests/integration/test_iso2_generated_xmldsig_roundtrip.py` around `PGPData__choice_choice_2` is removed; the scenario now passes alongside `PGPData__choice_choice_1` and `KeyInfo__choice_PGPData` in the xmldsig roundtrip suite.
- ADR-0007's choice_1 path is unaffected by patch 0002. Both patches modify disjoint regions of `iso2_msgDefDecoder.c` (0001 touches grammar 65 case 0 only; 0002 touches grammar 65 case 1 and grammar 69 case 2) and disjoint regions of the encoder (0001 leaves the encoder untouched; 0002 only edits PGPDataType paths).
- Any future re-pin of `extern/libcbv2g/` must keep both `patches/libcbv2g/0001-iso2-pgpdata.patch` and `patches/libcbv2g/0002-iso2-pgpdata-seq2.patch` applying cleanly. The Make target prints `skip` when each patch is already applied and aborts with a git-apply error if a patch no longer matches the new upstream, so a re-pin that breaks either fails the build instead of silently regressing.
- ADR-0001 ("codegen from libcbv2g headers") still holds: the EXPy-side marshalers are still generated from the vendored header, and patch 0002 does not change the header layout — it only touches the encoder/decoder grammar transitions and field references for PGPDataType.
