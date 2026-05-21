# EXI for Python (EXPy)

A Python and C/C++ binding over [LF Energy EVerest's
`libcbv2g`](https://github.com/EVerest/libcbv2g) providing per-Namespace EXI
encode/decode for V2G charging protocols (DIN 70121, ISO 15118-2,
ISO 15118-20). EXPy ships:

- A Python class (`EXIProcessor`) wrapping libcbv2g's eight per-Namespace
  codecs.
- A V2Gjson builder package (`expy.v2gjson`) — typed constructors and enums
  for building EVerest-shape JSON payloads.
- Eight CLI binaries — one per Namespace — for shell-driven encode/decode
  workflows.

## Namespaces

EXPy supports eight Namespaces. One Namespace maps to exactly one Processor
and one V2Gjson builder module.

| Namespace      | Standard                         | V2Gjson module             | CLI binary                       |
|----------------|----------------------------------|----------------------------|----------------------------------|
| `SAP`          | ISO 15118-2 Supported App Protocol | `expy.v2gjson.sap`         | `SupportedAppProtocolProcessor`  |
| `DIN`          | DIN 70121                        | `expy.v2gjson.din`         | `DINProcessor`                   |
| `ISO2`         | ISO 15118-2                      | `expy.v2gjson.iso2`        | `ISO2Processor`                  |
| `ISO20_COMMON` | ISO 15118-20 Common              | `expy.v2gjson.iso20_common`| `ISO20CommonProcessor`           |
| `ISO20_AC`     | ISO 15118-20 AC                  | `expy.v2gjson.iso20_ac`    | `ISO20ACProcessor`               |
| `ISO20_DC`     | ISO 15118-20 DC                  | `expy.v2gjson.iso20_dc`    | `ISO20DCProcessor`               |
| `ISO20_WPT`    | ISO 15118-20 WPT                 | `expy.v2gjson.iso20_wpt`   | `ISO20WPTProcessor`              |
| `ISO20_ACDP`   | ISO 15118-20 ACDP                | `expy.v2gjson.iso20_acdp`  | `ISO20ACDPProcessor`             |

## Installation

```bash
pip install git+https://github.com/IdahoLabResearch/EXPy@v1.0
```

Or build from a checkout:

```bash
git clone --recurse-submodules https://github.com/IdahoLabResearch/EXPy.git
cd EXPy
pip install .
```

If you cloned without `--recurse-submodules`:

```bash
git submodule update --init --recursive
```

`pip install .` invokes the `scikit-build-core` backend declared in
`pyproject.toml`, which drives the CMake build of `libcbv2g`, the eight
per-Namespace `.so` files, and the eight CLI binaries, then bundles them
into the installed `expy` package. CMake (≥3.20) and Ninja (≥1.10) must be
available at install time.

## Python API

### The six-method `EXIProcessor`

Each Processor exposes up to six methods, feature-gated by which roots the
underlying libcbv2g schema defines:

| Method                          | Root              | Where supported                                                 |
|---------------------------------|-------------------|-----------------------------------------------------------------|
| `encode(jsonObj) -> bytes`      | Document          | All eight Namespaces                                            |
| `decode(exiBytes) -> dict`      | Document          | All eight Namespaces                                            |
| `encode_fragment(jsonObj)`      | Fragment          | `ISO2`, `ISO20_COMMON`, `ISO20_AC`, `ISO20_DC`, `ISO20_WPT`, `ISO20_ACDP` |
| `decode_fragment(exiBytes)`     | Fragment          | Same six Namespaces                                             |
| `encode_xmldsig(jsonObj)`       | XmldsigFragment   | Same six Namespaces                                             |
| `decode_xmldsig(exiBytes)`      | XmldsigFragment   | Same six Namespaces                                             |

Fragment and XmldsigFragment methods are **absent** on Namespaces whose
schema does not define the corresponding root (`SAP` and `DIN` today).
Direct attribute access raises `AttributeError`. Feature-detect with
`hasattr`:

```python
from expy import EXIProcessor, Namespace

iso2Processor = EXIProcessor(Namespace.ISO2)
dinProcessor  = EXIProcessor(Namespace.DIN)

assert hasattr(iso2Processor, "encode_fragment")    # True
assert not hasattr(dinProcessor, "encode_fragment") # True — absent on DIN
```

### Document encode/decode

```python
import json
from expy import EXIProcessor, Namespace

dinProcessor = EXIProcessor(Namespace.DIN)

exiBytes = bytes.fromhex("809a021050908c0c0c0c0c51514002808142807c0c0c0000")
decoded = dinProcessor.decode(exiBytes)
print(json.dumps(decoded, indent=2))

reEncoded = dinProcessor.encode(decoded)
```

### Fragment encode/decode (PnC sub-elements)

```python
from expy import EXIProcessor, Namespace
from expy.v2gjson import iso2

iso2Processor = EXIProcessor(Namespace.ISO2)

authReq  = iso2.AuthorizationReqType(GenChallenge=bytearray(16))
fragment = iso2.exiFragment(AuthorizationReq=authReq)

encoded = iso2Processor.encode_fragment(fragment)
roundTripped = iso2Processor.decode_fragment(encoded)
```

### XmldsigFragment encode/decode (PnC signatures)

```python
from expy import EXIProcessor, Namespace
from expy.v2gjson import iso2

iso2Processor = EXIProcessor(Namespace.ISO2)

digestMethod = iso2.DigestMethodType(Algorithm="http://www.w3.org/2001/04/xmlenc#sha256")
canonMethod  = iso2.CanonicalizationMethodType(
    Algorithm="http://www.w3.org/TR/canonical-exi/"
)
sigMethod    = iso2.SignatureMethodType(
    Algorithm="http://www.w3.org/2001/04/xmldsig-more#ecdsa-sha256"
)
reference    = iso2.ReferenceType(DigestMethod=digestMethod, DigestValue=bytearray(32))
signedInfo   = iso2.SignedInfoType(
    CanonicalizationMethod=canonMethod,
    SignatureMethod=sigMethod,
    Reference=[reference],
)
xmldsig = iso2.xmldsigFragment(SignedInfo=signedInfo)
encoded = iso2Processor.encode_xmldsig(xmldsig)
```

## V2Gjson builders

`expy.v2gjson.<namespace>` modules expose typed constructors and enum
classes that emit EVerest-shape JSON. Identifier names mirror libcbv2g
verbatim — including non-PEP-8 casing such as `costKindType`,
`RelativeTimeIntervalType`, and `EVSE_NotReady`. Strict-linting consumers
can suppress per-file with ruff:

```python
# ruff: noqa: N801, N802, N806
```

**Never use wildcard imports across V2Gjson modules.** Every Namespace
defines `MessageHeaderType`, `BodyType`, `V2G_Message`, etc., which
silently collide on import order. Always import the module:

```python
from expy.v2gjson import din, iso2, iso20_common
```

### Top-level envelope shape varies per Namespace

V2Gjson constructors reflect each Namespace's schema verbatim — no
synthetic envelope is inserted.

**DIN and ISO-2** wrap top-level messages in `V2G_Message(Header=...,
Body=...)`:

```python
from expy.v2gjson import din

header = din.MessageHeaderType(SessionID=bytearray(b"DECAFBAD"))
status = din.DC_EVStatusType(
    EVReady=1, EVErrorCode=din.DC_EVErrorCodeType.NO_ERROR, EVRESSSOC=85
)
body = din.BodyType(CableCheckReq=din.CableCheckReqType(DC_EVStatus=status))
message = din.V2G_Message(Header=header, Body=body)
```

**SAP** has no `V2G_Message` — hand-build a raw dict at the top level:

```python
from expy.v2gjson import sap

message = {
    "supportedAppProtocolRes": sap.supportedAppProtocolRes(
        ResponseCode=sap.responseCodeType.OK_SuccessfulNegotiation,
        SchemaID=1,
    )
}
```

**ISO-20** has no `V2G_Message` — top-level messages are
`{"Body": {"<MessageName>": ...}}`:

```python
from expy.v2gjson import iso20_common

header = iso20_common.MessageHeaderType(
    SessionID=bytearray(b"DECAFBAD"), TimeStamp=1700000000
)
sessionSetup = iso20_common.SessionSetupReqType(Header=header, EVCCID="EVCC-001")
message = {"Body": {"SessionSetupReq": sessionSetup}}
```

## EVerest JSON shape

EXPy emits and accepts the JSON conventions inherited from libcbv2g:

- Byte fields: `{"bytes": [...], "bytesLen": N}`.
- Character fields: `{"characters": [...], "charactersLen": N}`.
- Optional sub-elements are signaled by JSON key presence (key omitted ↔
  the C struct's `_isUsed` bit is 0).

Example:

```json
{
    "Body": {
        "SessionSetupRes": {
            "EVSEID": {"bytes": [0], "bytesLen": 1},
            "ResponseCode": 0
        }
    },
    "Header": {
        "SessionID": {"bytes": [65, 66, 66, 48, 48, 48, 48, 49], "bytesLen": 8}
    }
}
```

## Error contract

`EXIProcessor` raises `EncodeError` and `DecodeError` on non-zero status
from the underlying C entry points. Both carry typed attributes:

- `rc: int | None` — return code. `-1` through `-299` are libcbv2g's
  `EXI_ERROR__*` constants; `-1000` is `EXPY_ERROR__MARSHALER_INPUT`
  (caught nlohmann JSON exception at the marshaler boundary).
- `namespace: str | None` — Namespace member name (e.g. `"DIN"`,
  `"ISO20_COMMON"`).
- `root: str | None` — `"exiDocument"`, `"exiFragment"`, or
  `"xmldsigFragment"`.

Discriminate on the attributes, not by substring-matching `str(e)`:

```python
from expy import EXIProcessor, Namespace, DecodeError

processor = EXIProcessor(Namespace.DIN)
try:
    processor.decode(b"\x00")
except DecodeError as e:
    if e.rc == -1000:
        ...  # marshaler-input rejection
    elif e.rc is not None and -299 <= e.rc <= -1:
        ...  # libcbv2g internal error
```

The `str(e)` message format is preserved for human-readable logs but is
**informational, not contractual**.

## CLI

The eight Processor binaries are installed as console scripts. After
`pip install .` they are on `PATH`. Each supports up to six modes,
feature-gated identically to the Python API:

| Mode                   | Where supported                                                  |
|------------------------|------------------------------------------------------------------|
| `--encode` / `-e`      | All eight binaries                                               |
| `--decode` / `-d`      | All eight binaries                                               |
| `--encode-fragment`    | `ISO2Processor`, `ISO20{Common,AC,DC,WPT,ACDP}Processor`         |
| `--decode-fragment`    | Same six                                                         |
| `--encode-xmldsig`     | Same six                                                         |
| `--decode-xmldsig`     | Same six                                                         |

Unsupported flags are absent from `--help` on `SupportedAppProtocolProcessor`
and `DINProcessor`; passing an unsupported flag exits with a
Namespace-specific error.

### Document mode (every binary)

```bash
DINProcessor --decode=809a021050908c0c0c0c0c51514002808142807c0c0c0000

DINProcessor --encode='{"Body": {"SessionSetupRes": {"EVSEID": {"bytes": [0], "bytesLen": 1}, "ResponseCode": 0}}, "Header": {"SessionID": {"bytes": [65, 66, 66, 48, 48, 48, 48, 49], "bytesLen": 8}}}'

DINProcessor -e -i input.json -o output.txt
```

### Fragment mode (capable binaries)

```bash
ISO2Processor --encode-fragment='{"AuthorizationReq": {"GenChallenge": {"bytes": [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0], "bytesLen": 16}}}'
```

### XmldsigFragment mode (capable binaries)

```bash
ISO2Processor --decode-xmldsig=$(< signed-info.exi.hex)
```

### General usage

```
Usage:
  ./EXIProcessor --[encode/decode]='STRING_OPTIONAL' -i FILE_INPUT -o FILE_OUTPUT [OPTION...]

  -h, --help                    Show help
  -e, --encode [=arg(=cmd)]     Encode EXI Document from JSON to EXI bytes
  -d, --decode [=arg(=cmd)]     Decode EXI Document from EXI bytes to JSON
      --encode-fragment ...     Encode EXI Fragment (capable binaries only)
      --decode-fragment ...     Decode EXI Fragment (capable binaries only)
      --encode-xmldsig ...      Encode EXI XmldsigFragment (capable binaries only)
      --decode-xmldsig ...      Decode EXI XmldsigFragment (capable binaries only)
  -i, --input arg               Input file (JSON for encode, hex EXI for decode)
  -o, --output arg               Output file
```

If no input/output file is given the binary reads from stdin and writes to
stdout. Use double quotes (`"`) and lowercase `true`/`false` in JSON
literals on the shell.

## Further reading

- `CHANGELOG.md` — v1.0.0 release notes.
- `CONTEXT.md` — glossary of project terminology.
- `docs/adr/` — Architecture Decision Records documenting the public
  surface (`0003`, `0012`), packaging (`0013`), and v1.0 refinements
  (`0014`).
- `example.py` — runnable end-to-end demonstration of the public surface.
