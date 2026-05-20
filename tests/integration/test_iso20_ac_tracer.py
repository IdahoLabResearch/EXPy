"""Hand-written ISO20_AC Processor tracer.

Stands alone from the schema-driven sweep (test_iso20_ac_generated_*):
one realistic `AC_ChargeParameterDiscoveryReq` Document round-trip that
exercises the full encode/decode path for the AC Namespace.
"""

import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from conftest import assert_roundtrip  # noqa: E402


def test_ac_charge_parameter_discovery_req_document_roundtrip():
    payload = {
        "AC_ChargeParameterDiscoveryReq": {
            "Header": {
                "SessionID": {
                    "bytesLen": 8,
                    "bytes": [13, 14, 12, 10, 15, 11, 10, 13],
                },
                "TimeStamp": 0,
            },
            "AC_CPDReqEnergyTransferMode": {
                "EVMaximumChargePower": {"Exponent": 0, "Value": 1},
                "EVMinimumChargePower": {"Exponent": 0, "Value": 1},
            },
        }
    }
    assert_roundtrip("ISO20_AC", payload)
