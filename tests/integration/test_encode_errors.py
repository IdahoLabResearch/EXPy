"""EncodeError contract: encode raises a typed exception on libcbv2g failure.

Pins the *strong* contract introduced by #20 — the weak rejection-envelope
contract for malformed payloads (None | raise | abort) lives in
``test_negative_inputs.py``.

The Processor uses a fixed 256-byte static buffer for the encoder bitstream;
a valid payload that encodes to more than 256 bytes hits
``EXI_ERROR__BITSTREAM_OVERFLOW`` from libcbv2g, which is the reachable RC
path we pin here.
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
