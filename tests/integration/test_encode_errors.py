"""EncodeError contract: encode raises a typed exception on libcbv2g failure
*and* on malformed-JSON / marshaler-input errors.

The first failure surface (libcbv2g rc) was wired up by #20. The second
(json::parse, marshaler [] and template get<T>() on untrusted input)
is wired up by #21 — both surfaces now raise ``EncodeError`` in the calling
process so consumers don't have to isolate encode in a subprocess.

Tests run in the parent process: the whole point of the hardening is that
malformed input no longer aborts; if it does, the regression must surface as
a pytest crash, not be hidden inside a subprocess.
"""

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from EXIProcessor import EncodeError, EXIProcessor, ProtocolEnum  # noqa: E402


def _ascii(s: str) -> dict:
    return {"charactersLen": len(s), "characters": [ord(c) for c in s]}


def test_sap_encode_overflow_raises_encode_error():
    # 5 AppProtocols × 100-char ProtocolNamespace (the schema max) — encodes
    # to well over the 256-byte static buffer, forcing BITSTREAM_OVERFLOW.
    namespace = "u" * 100  # 100 ASCII chars; the schema max
    one_protocol = {
        "ProtocolNamespace": _ascii(namespace),
        "VersionNumberMajor": 2,
        "VersionNumberMinor": 0,
        "SchemaID": 1,
        "Priority": 1,
    }
    payload = {
        "supportedAppProtocolReq": {
            "AppProtocol": {
                "arrayLen": 5,
                "array": [one_protocol] * 5,
            }
        }
    }

    processor = EXIProcessor(ProtocolEnum.SAP)
    with pytest.raises(EncodeError) as exc_info:
        processor.encode(payload)
    msg = str(exc_info.value)
    assert "SAP" in msg
    assert "exiDocument" in msg


def test_iso2_encode_missing_required_field_raises_encode_error():
    # V2G_Message with no Header — the marshaler's `["Header"]["SessionID"]`
    # accessor would have aborted via nlohmann's const operator[] on a missing
    # key before #21 landed. The hardening turns that abort into a typed
    # EncodeError in the calling process.
    processor = EXIProcessor(ProtocolEnum.ISO2)
    with pytest.raises(EncodeError) as exc_info:
        processor.encode({"V2G_Message": {}})
    msg = str(exc_info.value)
    assert "ISO2" in msg
    assert "exiDocument" in msg


def test_iso2_encode_invalid_json_payload_raises_encode_error():
    # Python's `json.dumps` accepts `float('nan')` and emits the non-standard
    # literal `NaN`, which nlohmann::json::parse then rejects with parse_error.
    # The hardening converts that C++ exception into EncodeError; previously the
    # exception would have unwound past the extern "C" boundary and aborted.
    processor = EXIProcessor(ProtocolEnum.ISO2)
    with pytest.raises(EncodeError) as exc_info:
        processor.encode({"V2G_Message": float("nan")})
    msg = str(exc_info.value)
    assert "ISO2" in msg
    assert "exiDocument" in msg


def test_din_encode_wrong_typed_scalar_raises_encode_error():
    # Header.SessionID expects a {"bytes": [...], "bytesLen": N} object; a bare
    # string violates the typed accessor (`template get<uint16_t>()` on a string
    # value would have aborted). Hardening turns it into EncodeError.
    processor = EXIProcessor(ProtocolEnum.DIN)
    with pytest.raises(EncodeError) as exc_info:
        processor.encode({"Header": {"SessionID": "not-bytes"}})
    msg = str(exc_info.value)
    assert "DIN" in msg
    assert "exiDocument" in msg
