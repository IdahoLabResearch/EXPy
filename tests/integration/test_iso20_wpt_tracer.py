"""Hand-written ISO20_WPT Processor tracer.

Stands alone from the schema-driven sweep (test_iso20_wpt_generated_*):
one realistic `WPT_PairingReq` Document round-trip that exercises the full
encode/decode path for the WPT Namespace.
"""

import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from conftest import assert_roundtrip  # noqa: E402


def test_wpt_pairing_req_document_roundtrip():
    payload = {
        "WPT_PairingReq": {
            "Header": {
                "SessionID": {
                    "bytesLen": 8,
                    "bytes": [13, 14, 12, 10, 15, 11, 10, 13],
                },
                "TimeStamp": 0,
            },
            "EVProcessing": 0,
            "EVResultCode": 1,
        }
    }
    assert_roundtrip("ISO20_WPT", payload)
