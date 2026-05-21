# Changelog

All notable changes to EXPy are documented in this file. The project adheres
to semantic versioning per ADR-0012 (extended by ADR-0014); the MAJOR-bump
list enumerates exactly which surface changes force a major version bump.

## v1.0.0

First tagged release. Public surface declared stable under the semver rules
of ADR-0012 + ADR-0014.

### Public API

- **Eight Namespaces** covered by per-Namespace Processor instances:
  `SAP`, `DIN`, `ISO2`, `ISO20_COMMON`, `ISO20_AC`, `ISO20_DC`, `ISO20_WPT`,
  `ISO20_ACDP`. Selected via the `Namespace` enum (`from expy import
  Namespace`).
- **Six-method `EXIProcessor` API**, feature-gated per Namespace:
  - `encode` / `decode` — Document root. Available on all eight Namespaces.
  - `encode_fragment` / `decode_fragment` — Fragment root. Available on
    `ISO2`, `ISO20_COMMON`, `ISO20_AC`, `ISO20_DC`, `ISO20_WPT`,
    `ISO20_ACDP`. Absent on `SAP` and `DIN` (their schemas define no
    `*_exiFragment` root).
  - `encode_xmldsig` / `decode_xmldsig` — XmldsigFragment root. Available on
    the same six Namespaces as Fragment.
  - Feature detection is `hasattr`-based: `hasattr(p, "encode_fragment")`
    returns `False` on unsupported Namespaces, and direct attribute access
    raises `AttributeError`.
- **Eight CLI binaries**, one per Namespace, installed as console scripts
  via `pip install`: `SupportedAppProtocolProcessor`, `DINProcessor`, `ISO2Processor`,
  `ISO20CommonProcessor`, `ISO20ACProcessor`, `ISO20DCProcessor`,
  `ISO20WPTProcessor`, `ISO20ACDPProcessor`. Each binary exposes up to six
  modes — `--encode`, `--decode`, `--encode-fragment`, `--decode-fragment`,
  `--encode-xmldsig`, `--decode-xmldsig` — feature-gated identically to the
  Python API. Unsupported flags are absent from `--help` on `SAP` and
  `DIN`; passing an unsupported flag exits with a Namespace-specific
  message rather than a generic parse failure.
- **V2Gjson builder modules** under `expy.v2gjson` — one module per
  Namespace (`sap`, `din`, `iso2`, `iso20_common`, `iso20_ac`, `iso20_dc`,
  `iso20_wpt`, `iso20_acdp`). Each exposes typed enum classes and
  constructor functions whose names mirror the underlying libcbv2g
  identifiers verbatim (including non-PEP-8 casing like `costKindType`,
  `RelativeTimeIntervalType`, `EVSE_NotReady`). Constructors emit dicts in
  the EVerest JSON shape, ready to pass to a Processor's encode methods.
- **Typed exception attributes** on `EncodeError` and `DecodeError`:
  - `rc: int | None` — integer return code from the C++ entry point.
    `-1` through `-299` are libcbv2g's `EXI_ERROR__*` constants; `-1000`
    is `EXPY_ERROR__MARSHALER_INPUT` (caught nlohmann JSON exception at
    the marshaler boundary).
  - `namespace: str | None` — the `Namespace` member name (`"SAP"`,
    `"DIN"`, `"ISO2"`, `"ISO20_COMMON"`, etc.).
  - `root: str | None` — `"exiDocument"`, `"exiFragment"`, or
    `"xmldsigFragment"`.
  - The `str(e)` message format from ADR-0006 is preserved for
    human-readable logs but is **informational, not contractual**.
    Consumers discriminating on failure mode branch on the attributes.

### Packaging

- **`pip install`-able distribution.** `pyproject.toml` at the repo root
  drives a scikit-build-core build that compiles libcbv2g, the eight
  per-Namespace `.so` files, and the eight CLI binaries, then bundles them
  as package data under the installed `expy/` package. Installation:
  - `pip install git+https://github.com/IdahoLabResearch/EXPy@v1.0`
  - `git clone --recurse-submodules <giturl> && cd EXPy && pip install .`
- **Source-tree layout**: `expy/` package with `processor.py` (was
  `EXIProcessor.py`), `_version.py`, and `v2gjson/` (was `V2Gjson/`).

### EVerest JSON shape

The JSON shape EXPy emits and accepts is inherited from libcbv2g
conventions and is stable under v1.0:

- Byte fields: `{"bytes": [...], "bytesLen": N}`.
- Character fields: `{"characters": [...], "charactersLen": N}`.
- Optional sub-elements are signaled by JSON key presence (key omitted ↔
  the C struct's `_isUsed` bit is 0).

### Top-level envelope split

Top-level message envelope shape varies per Namespace and is documented as
a known property of the surface:

- **DIN and ISO-2** wrap top-level messages in
  `<ns>.V2G_Message(Header=..., Body=...)` — the schemas define a
  `V2G_Message` complex type.
- **SAP and the five ISO-20 Namespaces** have no `V2G_Message`; top-level
  messages are hand-built raw dicts: `{"<MessageName>": ...}` for SAP,
  `{"Body": {"<MessageName>": ...}}` for ISO-20.

V2Gjson constructors reflect each Namespace's schema verbatim — no
synthetic envelope is inserted.

### Notes for consumers

- V2Gjson identifier casing mirrors libcbv2g verbatim. Strict-linting
  consumers can suppress per-file with ruff: `# ruff: noqa: N801, N802,
  N806`.
- Wildcard imports across `expy.v2gjson` modules are **not** supported —
  every Namespace defines `MessageHeaderType`, `BodyType`, `V2G_Message`,
  etc., which silently collide on import order. Always import the module:
  `from expy.v2gjson import din, iso2`.
