"""Cross-namespace public-surface parity (ADR-0012, issue #33).

Pins the invariant that every ``Namespace`` value has all three public
surfaces wired up: a Python ``EXIProcessor`` with the six-method API, a
``V2Gjson`` Namespace module with at least one constructor, and a CLI binary
under ``build/``. A future Namespace added to ``Namespace`` without all
three will fail this test.

Sibling tests cover individual surfaces in depth:
  - ``test_namespace_dispatch.py`` — ``Namespace`` membership and method callability.
  - ``test_unsupported_methods.py`` — which Namespaces raise on Fragment / XmldsigFragment.
  - ``test_cli_binaries.py`` — per-binary ``-h`` and hex roundtrip behavior.

This file is the integration cross-check across all three.
"""

from __future__ import annotations

import importlib
import subprocess
import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from EXIProcessor import EXIProcessor, Namespace  # noqa: E402

BUILD = REPO_ROOT / "build"

_BASE_METHODS = ("encode", "decode")
_GATED_METHODS = (
    "encode_fragment",
    "decode_fragment",
    "encode_xmldsig",
    "decode_xmldsig",
)

# Namespaces whose libcbv2g schema defines `*_exiFragment` and `xmldsigFragment`
# roots; the corresponding EXIProcessor methods are present iff a Namespace is
# listed here (ADR-0014 feature gating).
_FRAGMENT_CAPABLE = frozenset({"ISO2", "ISO20_COMMON", "ISO20_AC", "ISO20_DC"})

# The single constructor each V2Gjson Namespace module must expose. SAP is the
# outlier — it has no Header/Body envelope, so its tracer is `supportedAppProtocolReq`.
_V2GJSON_CONSTRUCTOR = {
    "SAP": "supportedAppProtocolReq",
}


def _v2gjson_constructor_for(name: str) -> str:
    return _V2GJSON_CONSTRUCTOR.get(name, "MessageHeaderType")


@pytest.mark.parametrize("protocol", list(Namespace), ids=lambda p: p.name)
def test_python_processor_surface(protocol):
    p = EXIProcessor(protocol)
    for method in _BASE_METHODS:
        assert callable(getattr(p, method)), (
            f"{protocol.name}: EXIProcessor missing {method}"
        )
    gated_present = protocol.name in _FRAGMENT_CAPABLE
    for method in _GATED_METHODS:
        assert hasattr(p, method) is gated_present, (
            f"{protocol.name}: {method} presence mismatch "
            f"(expected {gated_present}, got {hasattr(p, method)})"
        )
        if gated_present:
            assert callable(getattr(p, method))


@pytest.mark.parametrize("protocol", list(Namespace), ids=lambda p: p.name)
def test_v2gjson_module_surface(protocol):
    module_name = f"V2Gjson.{protocol.name.lower()}"
    module = importlib.import_module(module_name)
    constructor = _v2gjson_constructor_for(protocol.name)
    assert callable(getattr(module, constructor, None)), (
        f"{protocol.name}: {module_name} missing constructor `{constructor}`. "
        "Every Namespace module must expose at least one constructor function."
    )


@pytest.mark.parametrize("protocol", list(Namespace), ids=lambda p: p.name)
def test_cli_binary_surface(protocol):
    binary = BUILD / f"{protocol.value}Processor"
    assert binary.exists(), (
        f"{protocol.name}: missing CLI binary at {binary}. "
        "Every Namespace must ship a CLI binary (ADR-0012)."
    )
    result = subprocess.run(
        [str(binary), "-h"], capture_output=True, text=True, timeout=10
    )
    assert result.returncode == 0, (
        f"{protocol.name}: `{binary.name} -h` exited {result.returncode}\n"
        f"stderr:\n{result.stderr}"
    )
