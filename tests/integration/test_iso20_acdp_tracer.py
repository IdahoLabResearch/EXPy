"""Hand-written ISO20_ACDP Processor tracer.

Stands alone from the schema-driven sweep (test_iso20_acdp_generated_*):
one realistic `ACDP_ConnectReq` Document round-trip that exercises the full
encode/decode path for the ACDP Namespace.
"""

import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from conftest import assert_roundtrip  # noqa: E402


def test_acdp_connect_req_document_roundtrip():
    payload = {
        "ACDP_ConnectReq": {
            "Header": {
                "SessionID": {
                    "bytesLen": 8,
                    "bytes": [13, 14, 12, 10, 15, 11, 10, 13],
                },
                "TimeStamp": 0,
            },
            "EVElectricalChargingDeviceStatus": 1,
        }
    }
    assert_roundtrip("ISO20_ACDP", payload)
