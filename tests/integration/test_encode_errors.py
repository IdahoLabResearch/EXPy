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

from expy import EncodeError, EXIProcessor, Namespace  # noqa: E402


def _ascii(s: str) -> dict:
    return {"charactersLen": len(s), "characters": [ord(c) for c in s]}


# BITSTREAM_OVERFLOW is no longer reachable from schema-conformant SAP input
# after the encoder buffer bump (#47); the previous overflow test was removed.


def test_iso2_encode_missing_required_field_raises_encode_error():
    # V2G_Message with no Header — the marshaler's `["Header"]["SessionID"]`
    # accessor would have aborted via nlohmann's const operator[] on a missing
    # key before #21 landed. The hardening turns that abort into a typed
    # EncodeError in the calling process.
    processor = EXIProcessor(Namespace.ISO2)
    with pytest.raises(EncodeError) as exc_info:
        processor.encode({"V2G_Message": {}})
    err = exc_info.value
    assert err.namespace == "ISO2"
    assert err.root == "exiDocument"
    assert err.rc == -1000


def test_iso2_encode_invalid_json_payload_raises_encode_error():
    # Python's `json.dumps` accepts `float('nan')` and emits the non-standard
    # literal `NaN`, which nlohmann::json::parse then rejects with parse_error.
    # The hardening converts that C++ exception into EncodeError; previously the
    # exception would have unwound past the extern "C" boundary and aborted.
    processor = EXIProcessor(Namespace.ISO2)
    with pytest.raises(EncodeError) as exc_info:
        processor.encode({"V2G_Message": float("nan")})
    err = exc_info.value
    assert err.namespace == "ISO2"
    assert err.root == "exiDocument"
    # Marshaler-input failures use the EXPy sentinel rc, distinct from
    # libcbv2g's -1 .. -299 range.
    assert err.rc == -1000
    msg = str(err)
    assert "ISO2" in msg
    assert "exiDocument" in msg


def test_din_encode_wrong_typed_scalar_raises_encode_error():
    # Header.SessionID expects a {"bytes": [...], "bytesLen": N} object; a bare
    # string violates the typed accessor (`template get<uint16_t>()` on a string
    # value would have aborted). Hardening turns it into EncodeError.
    processor = EXIProcessor(Namespace.DIN)
    with pytest.raises(EncodeError) as exc_info:
        processor.encode({"Header": {"SessionID": "not-bytes"}})
    err = exc_info.value
    assert err.namespace == "DIN"
    assert err.root == "exiDocument"
    assert err.rc == -1000
