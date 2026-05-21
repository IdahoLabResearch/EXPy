"""Feature-gated Fragment / XmldsigFragment methods (ADR-0014).

EXIProcessor instances for Namespaces whose libcbv2g schema doesn't define
the corresponding root (today: SAP, DIN, ISO20_WPT, ISO20_ACDP) lack the
Fragment / XmldsigFragment methods entirely — ``hasattr`` returns False and
direct attribute access raises ``AttributeError``. Consumers feature-detect
via ``hasattr`` instead of try/except against ``NotImplementedError``.
"""

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from EXIProcessor import EXIProcessor, Namespace  # noqa: E402


_GATED_METHODS = (
    "encode_fragment",
    "decode_fragment",
    "encode_xmldsig",
    "decode_xmldsig",
)

_UNSUPPORTED = ("SAP", "DIN", "ISO20_WPT", "ISO20_ACDP")


@pytest.mark.parametrize("protocol_name", _UNSUPPORTED)
@pytest.mark.parametrize("method_name", _GATED_METHODS)
def test_gated_methods_absent_on_unsupported_namespaces(protocol_name, method_name):
    processor = EXIProcessor(Namespace[protocol_name])
    assert not hasattr(processor, method_name), (
        f"{protocol_name}: expected {method_name} to be absent"
    )
    with pytest.raises(AttributeError):
        getattr(processor, method_name)
