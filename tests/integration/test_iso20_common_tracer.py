"""Hand-written ISO20_COMMON Processor fixtures.

Stands alone from the schema-driven sweep (test_iso20_common_generated_*):
a tracer-bullet SessionSetupReq Document plus an explicit SignedInfo
XmldsigFragment — the latter is the PnC signature payload the issue called
out as the motivating use case for the xmldsig API surface.
"""

import json
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from conftest import assert_roundtrip  # noqa: E402


_FIXTURES = Path(__file__).parent / "fixtures" / "ISO20_COMMON"


def test_session_setup_req_document_roundtrip():
    payload = {
        "SessionSetupReq": {
            "Header": {
                "SessionID": {
                    "bytesLen": 8,
                    "bytes": [13, 14, 12, 10, 15, 11, 10, 13],
                },
                "TimeStamp": 0,
            },
            "EVCCID": {
                "charactersLen": 6,
                "characters": [ord(c) for c in "EVCC01"],
            },
        }
    }
    assert_roundtrip("ISO20_COMMON", payload)


def test_signed_info_xmldsig_fixture_roundtrip():
    """PnC signature payload root — `SignedInfo` is what the EVCC signs and
    the SECC verifies during PnC authorization. Distinct from the generated
    sweep: this fixture mirrors real-world W3C XML-DSig URIs (canonical-exi,
    ecdsa-sha256, xmlenc#sha256) so future schema or marshaler regressions
    that affect the realistic shape surface here before they hit consumers.
    """
    payload = json.loads((_FIXTURES / "SignedInfo.json").read_text())
    assert_roundtrip("ISO20_COMMON", payload, kind="xmldsig")
