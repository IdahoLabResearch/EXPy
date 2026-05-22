"""CLI binary parity per ADR-0012: one executable per Namespace under build/.

Exercises the public CLI surface (not the .so) — each binary must respond to
`-h` and round-trip a known-good hex stream via `--decode` / `--encode`.
"""

from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from expy import EXIProcessor, Namespace  # noqa: E402

BUILD = REPO_ROOT / "build"


_HEADER = {
    "SessionID": {"bytesLen": 8, "bytes": [13, 14, 12, 10, 15, 11, 10, 13]},
    "TimeStamp": 0,
}

# (binary_name, Namespace name, smoke-test payload). Payloads mirror the
# existing per-Namespace tracer fixtures (test_iso20_*_tracer.py) so the CLI
# smoke test exercises the same shape the lib-level tracer already covers.
_NEW_BINARIES = [
    (
        "ISO20CommonProcessor",
        "ISO20_COMMON",
        {
            "SessionSetupReq": {
                "Header": _HEADER,
                "EVCCID": {
                    "charactersLen": 6,
                    "characters": [ord(c) for c in "EVCC01"],
                },
            }
        },
    ),
    (
        "ISO20ACProcessor",
        "ISO20_AC",
        {
            "AC_ChargeParameterDiscoveryReq": {
                "Header": _HEADER,
                "AC_CPDReqEnergyTransferMode": {
                    "EVMaximumChargePower": {"Exponent": 0, "Value": 1},
                    "EVMinimumChargePower": {"Exponent": 0, "Value": 1},
                },
            }
        },
    ),
    (
        "ISO20DCProcessor",
        "ISO20_DC",
        {
            "DC_ChargeParameterDiscoveryReq": {
                "Header": _HEADER,
                "DC_CPDReqEnergyTransferMode": {
                    "EVMaximumChargePower": {"Exponent": 0, "Value": 1},
                    "EVMinimumChargePower": {"Exponent": 0, "Value": 1},
                    "EVMaximumChargeCurrent": {"Exponent": 0, "Value": 1},
                    "EVMinimumChargeCurrent": {"Exponent": 0, "Value": 1},
                    "EVMaximumVoltage": {"Exponent": 0, "Value": 1},
                    "EVMinimumVoltage": {"Exponent": 0, "Value": 1},
                },
            }
        },
    ),
    (
        "ISO20WPTProcessor",
        "ISO20_WPT",
        {
            "WPT_PairingReq": {
                "Header": _HEADER,
                "EVProcessing": 0,
                "EVResultCode": 1,
            }
        },
    ),
    (
        "ISO20ACDPProcessor",
        "ISO20_ACDP",
        {
            "ACDP_ConnectReq": {
                "Header": _HEADER,
                "EVElectricalChargingDeviceStatus": 1,
            }
        },
    ),
]


@pytest.mark.parametrize("binary_name,_proto,_payload", _NEW_BINARIES)
def test_cli_binary_responds_to_help(binary_name, _proto, _payload):
    binary = BUILD / binary_name
    assert binary.exists(), f"missing CLI binary: {binary}"
    result = subprocess.run(
        [str(binary), "-h"], capture_output=True, text=True, timeout=10
    )
    assert result.returncode == 0, result.stderr
    assert "Encode and decode V2G EXI Documents" in result.stdout


@pytest.mark.parametrize("binary_name,proto,payload", _NEW_BINARIES)
def test_cli_binary_roundtrips_known_good_hex(binary_name, proto, payload, tmp_path):
    binary = BUILD / binary_name
    original_hex = EXIProcessor(Namespace[proto]).encode(payload).hex()

    hex_in = tmp_path / "in.hex"
    hex_in.write_text(original_hex)
    json_mid = tmp_path / "mid.json"
    hex_out = tmp_path / "out.hex"

    decoded = subprocess.run(
        [str(binary), "--decode", "-i", str(hex_in), "-o", str(json_mid)],
        capture_output=True, text=True, timeout=10,
    )
    assert decoded.returncode == 0, decoded.stderr
    json.loads(json_mid.read_text())

    encoded = subprocess.run(
        [str(binary), "--encode", "-i", str(json_mid), "-o", str(hex_out)],
        capture_output=True, text=True, timeout=10,
    )
    assert encoded.returncode == 0, encoded.stderr
    assert hex_out.read_text().strip() == original_hex
