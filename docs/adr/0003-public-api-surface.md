# Public API Surface: EXIProcessor and EVerest JSON Shape Only

EXPy's public, stable surface is `EXIProcessor`, `ProtocolEnum`, the six encode/decode methods on a Processor (Document, Fragment, XmldsigFragment — each with encode and decode), and the EVerest JSON shape produced and consumed by those methods. Everything under `V2Gjson/` (enum classes like `costKindType`, `paymentOptionType`, helper modules) is **internal** and may change without notice. Consumers that want typed enums should generate their own from `libcbv2g` headers, or accept the enum values as raw integers per the EVerest JSON shape.

## Considered Options

- **Broad surface** including all of `V2Gjson/` as public typed constants. Rejected: couples EXPy to consumers' domain models, makes every libcbv2g enum churn a breaking change, and the `V2Gjson/` modules were never designed as a public API.
- **Codec-only** — only the `.so` ABI and JSON shape are public; `EXIProcessor` is a reference wrapper consumers are free to replace. Rejected: under-promises and forces every consumer to redo the ctypes wrapper.

## Consequences

- `V2Gjson/` can be reorganized, renamed, or regenerated freely without bumping major version.
- AcCCS (and similar consumers) translate at their boundary between the EVerest JSON shape and their own internal representation, rather than importing EXPy domain types directly.
- Documentation (README, future API reference) calls out exactly this boundary so consumers don't accidentally depend on internals.
