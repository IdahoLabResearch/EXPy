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
    err = exc_info.value
    assert err.namespace == "ISO2"
    assert err.root == "exiDocument"
    # Empty bitstream fails inside libcbv2g, so rc is in its negative range
    # (-1 .. -299), not the EXPy marshaler-input sentinel (-1000).
    assert isinstance(err.rc, int)
    assert err.rc < 0
    assert err.rc != -1000
    # ADR-0006 message format is preserved for human logs.
    msg = str(err)
    assert "ISO2" in msg
    assert "exiDocument" in msg


def test_din_decode_empty_bytes_raises_decode_error():
    processor = EXIProcessor(ProtocolEnum.DIN)
    with pytest.raises(DecodeError) as exc_info:
        processor.decode(b"")
    err = exc_info.value
    assert err.namespace == "DIN"
    assert err.root == "exiDocument"
    assert isinstance(err.rc, int) and err.rc < 0 and err.rc != -1000


def test_sap_decode_empty_bytes_raises_decode_error():
    processor = EXIProcessor(ProtocolEnum.SAP)
    with pytest.raises(DecodeError) as exc_info:
        processor.decode(b"")
    err = exc_info.value
    assert err.namespace == "SAP"
    assert err.root == "exiDocument"
    assert isinstance(err.rc, int) and err.rc < 0 and err.rc != -1000


def test_iso2_decode_fragment_empty_bytes_raises_decode_error():
    processor = EXIProcessor(ProtocolEnum.ISO2)
    with pytest.raises(DecodeError) as exc_info:
        processor.decode_fragment(b"")
    err = exc_info.value
    assert err.namespace == "ISO2"
    assert err.root == "exiFragment"
    assert isinstance(err.rc, int) and err.rc < 0 and err.rc != -1000


def test_iso2_decode_xmldsig_empty_bytes_raises_decode_error():
    processor = EXIProcessor(ProtocolEnum.ISO2)
    with pytest.raises(DecodeError) as exc_info:
        processor.decode_xmldsig(b"")
    err = exc_info.value
    assert err.namespace == "ISO2"
    assert err.root == "xmldsigFragment"
    assert isinstance(err.rc, int) and err.rc < 0 and err.rc != -1000


@pytest.mark.parametrize("protocol_name", ["SAP", "DIN", "ISO2"])
@pytest.mark.parametrize("seed", range(20))
def test_decode_arbitrary_bytes_never_aborts(protocol_name, seed):
    # Residual marshaler-abort coverage (#21). The libcbv2g rc path (#20) is
    # already pinned above; this layer pins the *second* failure surface —
    # any nlohmann assertion / parse / type error reached during decode must
    # surface as DecodeError in the calling process, not abort the runner.
    # We feed pseudo-random byte patterns of varying lengths; the assertion is
    # structural ("dict or DecodeError"), since we have no specific input that
    # is known to libcbv2g-accept but marshaler-reject.
    import random
    rng = random.Random(seed)
    buf = bytes(rng.randint(0, 255) for _ in range(rng.randint(1, 64)))
    processor = EXIProcessor(ProtocolEnum[protocol_name])
    try:
        result = processor.decode(buf)
    except DecodeError:
        return
    assert isinstance(result, dict)
