# Vendored libcbv2g patch: ISO20_WPT TxSpecData / RxSpecData / PulseSequenceOrder loop-grammar (#26)

ADR-0007 and ADR-0008 patched ISO-2 PGPData grammars. This ADR covers a structurally different defect class in the ISO20_WPT codec: bounded-LOOP grammars that the upstream cbexigen generator emits without an exit transition. Three scenarios in `tests/integration/test_iso20_wpt_generated_roundtrip.py` (`WPT_FinePositioningSetupReq__choice_ANY/_XPath`, `WPT_FinePositioningSetupRes__choice_ANY/_XPath`) were strict-`xfail` against #26 until this patch landed.

Three independent upstream LOOP grammars in libcbv2g's iso20_WPT codec share the same shape: a 1-bit pure-LOOP state where the array-exhausted branch returns `EXI_ERROR__UNKNOWN_EVENT_CODE` instead of transitioning to the next grammar. Once any of these arrays has a populated tail, the encoder cannot exit and the round-trip aborts.

1. **`encode_iso20_wpt_WPT_LF_TransmitterDataType` grammar 82 (`lib/cbv2g/iso_20/iso20_WPT_Encoder.c`).** LOOP `TxSpecData`, `read/write bits=1`. Continuation emits 1-bit value 0 and stays at 82; the array-exhausted else-branch returns `EXI_ERROR__UNKNOWN_EVENT_CODE`. The XSD particle is `TxSpecData, WPT_TxRxSpecDataType (2, 255)` — minOccurs >= 2 — so any conforming payload that reaches grammar 82 must exit through it eventually.
2. **`encode_iso20_wpt_WPT_LF_ReceiverDataType` grammar 90.** Symmetric defect on the `RxSpecData (2, 255)` array. `RxSpecData` is the last particle of `WPT_LF_ReceiverDataType`, so the exit transition is directly to END Element instead of to a successor grammar.
3. **`encode_iso20_wpt_WPT_TxRxPackageSpecDataType` grammar 75.** Same shape on the `PulseSequenceOrder, WPT_TxRxPulseOrderType (2, 255)` array. Reached when `LF_TransmitterSetupData.TxPackageSpecData` is populated, which the maximal fixture inflates inside the FinePositioningSetup scenarios.

The decoders mirror the same defect: grammar 75 / 82 / 90 each handle `case 0` (LOOP continuation) and fall through to `EXI_ERROR__UNKNOWN_EVENT_CODE` for any non-zero event code, so even if the encoder emitted an exit bit the decoder would refuse it.

## Patch

The fix lives in EXPy at `patches/libcbv2g/0003-iso20-wpt-loop-grammar.patch`, applied alongside the ADR-0007 and ADR-0008 patches by the `libcbv2g` Make target. The auto-discovery wildcard in the Makefile (`LIBCBV2G_PATCHES = $(sort $(wildcard patches/libcbv2g/*.patch))`) picks the new patch up without configuration changes. The submodule remains pristine at its pinned upstream commit; the apply step probes with `git apply --check --reverse` first, so re-running `make` after the patches are in place prints `skip … (already applied)`.

Two files inside the submodule's working tree are modified by patch 0003:

- `lib/cbv2g/iso_20/iso20_WPT_Encoder.c`: grammars 75, 82, and 90 each replace the `error = EXI_ERROR__UNKNOWN_EVENT_CODE;` exit branch with `error = exi_basetypes_encoder_nbit_uint(stream, 1, 1);` followed by a grammar-id transition. Grammar 75 → 76 (PulseSeparationTime); grammar 82 → 83 (TxPackageSpecData / END Element); grammar 90 sets `done = 1; grammar_id = 3;` because `RxSpecData` is the final particle.
- `lib/cbv2g/iso_20/iso20_WPT_Decoder.c`: grammars 75, 82, and 90 each add a `case 1:` arm that performs the symmetric grammar-id transition. No state on the decoded struct is set by these arms — they consume the exit event and move to the next grammar to read.

Each patched site carries an inline `EXPy patch (ADR-0009): …` comment, mirroring ADR-0007/0008. A grep for `EXPy patch` against an applied submodule tree surfaces every divergence across all three ADRs.

The bit-stream layout for these three array fields gains one extra `1` bit at the LOOP-exit boundary compared to the unpatched upstream output. Round-trips are consistent because both sides are patched symmetrically; interop with external EXI implementations of ISO 15118-20 WPT was already accepted as out-of-scope by the broader patch lineage (see ADR-0008).

## Scope

The patch is iso20_WPT-only and confined to the three bounded-LOOP grammars listed above. The other LOOP grammars in the WPT codec (grammars 51, 121, 125, 135, 139, and the 142/144/146/148/150 family for `TaxCosts`) are emitted with `bits=2` and include the exit event natively in the upstream output — they are not touched. The DIN, ISO-2, ISO20_AC, ISO20_DC, ISO20_ACDP, and ISO20_COMMON codecs are not affected by this patch.

The defect almost certainly exists in the symmetric `WPT_HF_*` LOOP grammars and in the `iso20_acdp` LF/HF grammars, but none of the existing scenarios reach those paths, and the four scenarios this ADR targets are the only ones #26 enumerates as acceptance criteria. Patching speculatively would risk introducing transitions that have no test coverage. Follow-up patches can extend the same approach when those scenarios surface.

## Considered Options

- **Widen grammars 75 / 82 / 90 to `bits=2` and fold their successor grammars (76 / 83 / direct-END) into the LOOP state.** Rejected. Changes the bit width that the decoder reads, which forces a deeper rewrite of the LOOP-breakout logic (the upstream `arrayLen < 255 → grammar_id = -1` shortcut would also need to learn the new exit code). The current 1-bit patch preserves the upstream grammar shape and the breakout sentinel, keeping the diff surgical and easy to compare against future upstream re-pins.
- **Patch only grammars 82 and 90 (the two the #26 issue enumerates) and leave 75 broken.** Rejected. The FinePositioningSetup maximal fixture inflates `TxPackageSpecData`, which reaches grammar 75. Without all three patched, the four targeted scenarios would still fail with `rc=-150`.
- **Regenerate libcbv2g from its own schema.** Rejected for the same reasons as in ADR-0007 and ADR-0008. The upstream code generator emits the broken grammar tables; touching the generator is a much larger change that needs to be upstreamed. The hand-patch is a surgical local fix until that work happens.
- **Drop the four FinePositioningSetup scenarios from the round-trip suite.** Rejected. They are the only scenarios that exercise the `LF_SystemSetupData` → `LF_TransmitterSetupData` / `LF_ReceiverSetupData` paths, which together cover ~6% of the WPT message-type surface. Dropping them would leave the loop-grammar defect masked.

## Consequences

- The strict-`xfail` block `_LOOP_GRAMMAR_XFAILS` in `tests/integration/test_iso20_wpt_generated_roundtrip.py` collapses to `frozenset()`; the four scenarios now pass alongside the rest of the WPT document sweep. The xfail-routing branch in `_param` becomes a no-op but is left in place so the `_NESTED_CHOICE_XFAILS` branch keeps its parallel structure (the comment above it still references #25).
- Any future re-pin of `extern/libcbv2g/` must keep all three patches (`0001-iso2-pgpdata.patch`, `0002-iso2-pgpdata-seq2.patch`, `0003-iso20-wpt-loop-grammar.patch`) applying cleanly. The Make target prints `skip` when each patch is already applied and aborts with a `git apply` error if a patch no longer matches the new upstream, so a re-pin that breaks any patch fails the build instead of silently regressing.
- ADR-0001 ("codegen from libcbv2g headers") still holds: the EXPy-side marshalers are still generated from the vendored datatypes header, and patch 0003 does not touch any header — it only edits encoder/decoder LOOP transitions.
- The bit-stream-incompatibility footnote from ADR-0008 extends here: streams produced by an EXPy-patched libcbv2g for `WPT_LF_TransmitterDataType`, `WPT_LF_ReceiverDataType`, or `WPT_TxRxPackageSpecDataType` will not decode with an unpatched upstream libcbv2g. Within EXPy, both sides of every round-trip use the patched codec, so this is invisible to users; cross-implementation interop for these three types remains out of scope until the upstream grammar generator is fixed.
