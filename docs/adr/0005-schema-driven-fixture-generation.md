# Schema-driven fixture generation for integration roundtrip tests

Integration roundtrip tests previously used hand-written JSON fixtures (one file per scenario) compared via `_ordered()`, which sorted lists before comparing — silently passing cert-chain reorderings and any list-order corruption. Coverage was sparse: ISO2 had 3 of ~34 Documents, 1 of 10 Fragment roots, 1 of 19 Xmldsig roots. We are switching to a schema-driven generator that consumes `tools/codegen/parser.py` output and emits, per type, a minimal scenario (only required fields), a maximal scenario (all optionals present), and one scenario per XSD-choice branch — applied uniformly to Documents, Fragments, and Xmldsig roots across DIN and ISO2. Generated payloads are computed at test time (no committed JSON tree) alongside a small `overrides/` directory for hand-curated regression anchors and length-constrained fields the generator can't satisfy. Roundtrip equality becomes strict `==`. Enum values are sourced from `V2Gjson/*.py`; other scalars use per-kind defaults.

## Considered Options

- **Keep hand-written JSON fixtures.** Rejected: reaching the optional-present/absent + per-choice-branch coverage bar would require ~150 hand-edited files for ISO2 alone, and "every field is covered" remains a manual audit.
- **Use the `V2Gjson` builder library directly in tests.** Rejected as primary source: `V2Gjson` is hand-maintained and can drift from `libcbv2g` headers without surfacing as a test failure; it is retained as the enum-value source only, where drift fails loudly (generator falls back to 0, encode rejects invalid enum, test fails).
- **Full property-based generation (Hypothesis).** Rejected for this push: the infrastructure cost to produce *valid* (not just well-typed) V2G payloads is significant, and a deterministic min/max/per-branch matrix already meets the stated coverage goal.

## Consequences

- `tests/integration/conftest.py` drops `_ordered()` and compares with plain `==`; cert-chain and schedule-entry order changes will fail loud. Any pre-existing fixture that only passed because of sort-tolerance surfaces as a real bug.
- A new `tools/codegen/fixture_emitter.py` (or similar) enumerates `(namespace, root_kind, type, variant)` tuples and emits payloads. Roundtrip tests parametrize over the enumeration; failure IDs name the scenario (`ChargeParameterDiscoveryReq__choice_DC_EVChargeParameter`).
- The generator must know which scalar fields are enums. The parser is extended (or a sidecar table is added) to harvest `typedef enum { ... }` declarations from the same headers; enum-typed fields seed from `V2Gjson`'s `Enum` classes.
- Length-constrained fields (e.g., `SessionID` exactly 8 bytes, `EVCCID` exactly 6 bytes) cannot be inferred from the C headers and fail encode if seeded with the per-kind default. These need entries in a small per-field seed-override table.
- ISO20 namespaces are out of scope for this push — they gain coverage when their Processors land. The generator is namespace-parametric, so ISO20 fixture coverage is mechanical once those headers are wired in.
- Negative tests (malformed JSON / malformed EXI) are a smoke layer (~3 per namespace) in this push; a full negative matrix is deferred and depends on a separate decision about the encode/decode error contract.
