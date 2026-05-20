# Public API Surface: Python EXIProcessor, V2Gjson Builders, and Per-Namespace CLI Binaries

**Supersedes ADR-0003.**

EXPy's public, stable surface for v1.0 has three parts: (1) the Python `EXIProcessor` class with its `ProtocolEnum` and six encode/decode methods (Document, Fragment, XmldsigFragment); (2) the `V2Gjson` package — one module per Namespace, each exposing typed enum classes and constructor functions that emit the EVerest JSON shape; and (3) one CLI binary per Namespace (eight total: `DINProcessor`, `ISO2Processor`, `SupportedAppProtocolProcessor`, `ISO20CommonProcessor`, `ISO20ACProcessor`, `ISO20DCProcessor`, `ISO20WPTProcessor`, `ISO20ACDPProcessor`). Python (EXIProcessor + V2Gjson) is the primary surface; the CLI is a lower-priority convenience for shell/scripting use. The generated C++ headers (`src/generated/*.generated.hpp`) remain internal — a build artifact, not a supported embedding surface.

## Considered Options

- **Python `EXIProcessor` only** (the previous ADR-0003 stance). Rejected: the README has advertised the CLI from day one, AcCCS-adjacent workflows lean on it for ad-hoc hex-stream debugging, and consumers without V2Gjson constructors would have to re-implement the EVerest JSON shape envelope rules themselves.
- **Single dispatcher CLI** (`EXIProcessor --namespace=ISO20_AC ...`). Rejected for the same reason ADR-0002 rejected a mega-Processor `.so`: it forces one binary to link every libcbv2g codec, opaque string-dispatch, and breaks the one-Namespace ↔ one-Processor ↔ one-`.so` ↔ one-exec symmetry.
- **V2Gjson stays internal** (ADR-0003's stance, retained in 0012's first draft). Rejected: AcCCS would re-implement the same enum tables and constructors locally, defeating the point of shipping a Python binding. The schema-churn coupling is accepted as the cost of doing business — the EVerest JSON shape is already a public contract, so libcbv2g schema changes already break consumers; V2Gjson just makes that breakage typed.
- **C++ embedding as a fourth public surface.** Rejected for v1.0: no named C++ consumer, and committing to `.hpp` stability across libcbv2g revisions is non-trivial. Generated headers remain a build artifact only.

## Consequences

- The Makefile gains five exec targets (`ISO20Common`, `ISO20AC`, `ISO20DC`, `ISO20WPT`, `ISO20ACDP`Processor), mirroring the existing `.so` layout.
- Each CLI binary stays lean — only its Namespace's libcbv2g archive linked, same as its sibling `.so`.
- `tools/codegen/v2gjson_emitter.py` is extended to emit constructor functions alongside enum classes. The hand-written `V2Gjson/{din,iso2,common,apphand}.py` modules are regenerated through that extended emitter so all 8 Namespaces follow one codegen path.
- `V2Gjson/__init__.py` re-exports all 8 Namespace modules.
- Library API stability is tied to the libcbv2g schema. The following changes require a major version bump (semver MAJOR):
  - Any `V2Gjson` constructor signature change (arg added/removed/reordered, required→optional, type change).
  - Any `V2Gjson` enum value renamed or removed.
  - Any change to the EVerest JSON shape (key rename, type change, optional↔required flip) on an existing field.
  - Any `EXIProcessor` method signature change.

  Additive changes (a new `ProtocolEnum` value, a new optional field, a new enum value) are minor-version-safe. The single canonical version string lives in `_version.py` and is imported by both `EXIProcessor.py` and `V2Gjson/__init__.py`.
- The README documents all three public surfaces and explicitly marks `include/` / `src/generated/*.hpp` as internal.
- Stale hand-written headers in `include/` (`DINProcessor.hpp`, `ISO2Processor.hpp`, `SupportedAppProtocolProcessor.hpp`) are removed; they were pre-codegen artifacts (no `status` field, no Fragment/XmldsigFragment) and not `#include`'d anywhere.
