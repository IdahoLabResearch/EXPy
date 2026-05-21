"""Eight-Namespace dispatch shape — tracer bullet for ADR-0002.

Pins the public Namespace surface: every name listed in `CONTEXT.md` is a
`Namespace` member and (when implemented) loads a Processor exposing the
six-method API. Namespaces whose `.so` does not yet exist are marked xfail
here so the test self-extends as #4-#7 land.
"""

from __future__ import annotations

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from EXIProcessor import EXIProcessor, Namespace  # noqa: E402


_EXPECTED_NAMES = (
    "SAP",
    "DIN",
    "ISO2",
    "ISO20_COMMON",
    "ISO20_AC",
    "ISO20_DC",
    "ISO20_WPT",
    "ISO20_ACDP",
)

# Namespaces whose Processor .so is not yet built. These remain xfail until the
# corresponding implementation issue lands; they will fail strictly once the
# .so exists, at which point the entry is removed.
_PENDING: frozenset[str] = frozenset()

# Namespaces whose libcbv2g schema defines `*_exiFragment` and `xmldsigFragment`
# roots — feature-gated EXIProcessor methods are present iff a Namespace is
# listed here (ADR-0014).
_FRAGMENT_CAPABLE = frozenset({"ISO2", "ISO20_COMMON", "ISO20_AC", "ISO20_DC"})


def test_protocol_enum_has_eight_namespaces():
    assert tuple(p.name for p in Namespace) == _EXPECTED_NAMES


@pytest.mark.parametrize(
    "name",
    [
        pytest.param(
            n,
            marks=pytest.mark.xfail(
                reason=f"{n} Processor .so lands in a later issue",
                strict=True,
            ),
        )
        if n in _PENDING
        else n
        for n in _EXPECTED_NAMES
    ],
)
def test_processor_loads_and_exposes_six_method_api(name):
    p = EXIProcessor(Namespace[name])
    for attr in ("encode", "decode"):
        assert callable(getattr(p, attr)), attr
    gated_present = name in _FRAGMENT_CAPABLE
    for attr in (
        "encode_fragment",
        "decode_fragment",
        "encode_xmldsig",
        "decode_xmldsig",
    ):
        assert hasattr(p, attr) is gated_present, (
            f"{name}: {attr} presence mismatch (expected {gated_present})"
        )
