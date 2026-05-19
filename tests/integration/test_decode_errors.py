"""DecodeError contract: decode raises a typed exception on libcbv2g failure.

Pins the *strong* contract introduced by #20 — the weak rejection-envelope
contract (None | raise | abort) lives in ``test_negative_inputs.py`` and
matches the original ADR-0006 floor.

Tests run in the parent process: the whole point of the hardening is that
malformed input no longer aborts; if it does, the regression must surface as
a pytest crash, not be hidden inside a subprocess.
"""

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from EXIProcessor import DecodeError, EXIProcessor, ProtocolEnum  # noqa: E402


def test_iso2_decode_empty_bytes_raises_decode_error():
    processor = EXIProcessor(ProtocolEnum.ISO2)
    with pytest.raises(DecodeError) as exc_info:
        processor.decode(b"")
    # The message must name the namespace and root so an operator can
    # tell which Processor and entry point failed without a stack trace.
    msg = str(exc_info.value)
    assert "ISO2" in msg
    assert "exiDocument" in msg


def test_din_decode_empty_bytes_raises_decode_error():
    processor = EXIProcessor(ProtocolEnum.DIN)
    with pytest.raises(DecodeError) as exc_info:
        processor.decode(b"")
    msg = str(exc_info.value)
    assert "DIN" in msg
    assert "exiDocument" in msg


def test_sap_decode_empty_bytes_raises_decode_error():
    processor = EXIProcessor(ProtocolEnum.SAP)
    with pytest.raises(DecodeError) as exc_info:
        processor.decode(b"")
    msg = str(exc_info.value)
    assert "SAP" in msg
    assert "exiDocument" in msg


def test_iso2_decode_fragment_empty_bytes_raises_decode_error():
    processor = EXIProcessor(ProtocolEnum.ISO2)
    with pytest.raises(DecodeError) as exc_info:
        processor.decode_fragment(b"")
    msg = str(exc_info.value)
    assert "ISO2" in msg
    assert "exiFragment" in msg


def test_iso2_decode_xmldsig_empty_bytes_raises_decode_error():
    processor = EXIProcessor(ProtocolEnum.ISO2)
    with pytest.raises(DecodeError) as exc_info:
        processor.decode_xmldsig(b"")
    msg = str(exc_info.value)
    assert "ISO2" in msg
    assert "xmldsigFragment" in msg
