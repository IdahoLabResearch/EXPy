# Canonicalize the SAP Namespace Name

The first Namespace was originally `HANDSHAKE` in `ProtocolEnum` and `AppHandshake` in source/build artifacts — a libcbv2g-derived name borrowed from its `appHand_*` header symbols. The V2G spec, ISO 15118-2 §8.2.4 ("Supported App Protocol"), and AcCCS's namespace-string dispatcher (see ADR-0002) all refer to this Namespace as `SAP` / `SupportedAppProtocol`. We are canonicalizing on `SAP` (`ProtocolEnum.SAP` with value `"SupportedAppProtocol"`) and renaming the Processor, source, header, `.so`, executable, fixtures, and integration test accordingly. The hand-written marshalers in this Processor are also replaced by codegen output per ADR-0001 — the bug class motivating this refurbish (`supportedAppProtocolReq` crashed on a missing `.contains()` guard) is the same one ADR-0001 cites for DIN.

## Considered Options

- **Keep `HANDSHAKE` / `AppHandshake`.** Rejected: drifts from the V2G spec, from AcCCS's namespace-string vocabulary, and from CONTEXT.md's guidance — every consumer would have to translate.
- **Add a back-compat shim** (`HANDSHAKE = SAP`, keep `lib-AppHandshakeProcessor.so` as a symlink). Rejected: per repo guidance, breaking changes propagate cleanly. The downstream impact is a single pin in AcCCS, tracked separately.

## Consequences

- `ProtocolEnum.HANDSHAKE` is removed; `ProtocolEnum.SAP = "SupportedAppProtocol"` replaces it. The Python `EXIProcessor` loader keeps its `lib-{value}Processor.so` convention, so the `.so` resolves to `lib-SupportedAppProtocolProcessor.so`.
- `src/AppHandshakeProcessor.cpp` → `src/SupportedAppProtocolProcessor.cpp`, `include/AppHandshakeProcessor.hpp` → `include/SupportedAppProtocolProcessor.hpp`, header guard updated.
- Makefile builds `lib-SupportedAppProtocolProcessor.so` and the `SupportedAppProtocolProcessor` executable. A new codegen target emits `src/generated/SAP_marshalers.generated.cpp` from `appHand_Datatypes.h`.
- The integration test is `test_sap_roundtrip` over `tests/integration/fixtures/SAP/`.
- AcCCS (`IdahoLabResearch/AcCCS`) pins `ProtocolEnum.HANDSHAKE` and loads `lib-AppHandshakeProcessor.so` — this is a breaking change for that consumer. A separate tracking issue belongs in AcCCS.
