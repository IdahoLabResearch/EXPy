# Eight Namespace-Granular Processors

`ProtocolEnum` currently has four values (`HANDSHAKE`, `DIN`, `ISO2`, `ISO20`) and the build produces four `.so` files. Consumers — notably AcCCS, see `IdahoLabResearch/AcCCS` ADR-0002 — dispatch encoding/decoding on eight distinct V2G namespace strings: `SAP`, `DIN_MSG_DEF`, `ISO_V2_MSG_DEF`, `ISO_V20_COMMON_MSG`, `ISO_V20_AC`, `ISO_V20_DC`, `ISO_V20_WPT`, `ISO_V20_ACDP`. We are expanding `ProtocolEnum` to eight values and building one `.so` per Namespace, mirroring `libcbv2g`'s own layering (which already splits ISO‑20 into five separate Encoder/Decoder pairs). One Namespace ↔ one Processor ↔ one `.so`.

## Considered Options

- **Keep four Processors, add an internal service selector for ISO‑20** (e.g., `encode(jsonObj, service="AC")`). Rejected: hides the split inside one larger `.so`, couples the API to a string contract, and complicates the JSON root-type story (each service has its own document type).
- **Single mega-Processor taking the namespace string directly.** Rejected: one large `.so` linking every libcbv2g codec, opaque dispatch, and forces consumers into a string contract.

## Consequences

- The Makefile grows from 3 `.so` targets to 8. Each `.so` links only the libcbv2g codec(s) for its Namespace, keeping link surface lean.
- AcCCS's namespace-string dispatcher reduces to a flat string → `ProtocolEnum` table.
- Adding a future Namespace (e.g., a future ISO revision) is mechanical: one new enum value, one new Processor, one new Makefile target.
