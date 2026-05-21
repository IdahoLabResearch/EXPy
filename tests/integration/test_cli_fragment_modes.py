"""CLI Fragment / XmldsigFragment modes per ADR-0014.

Each `*Processor` binary gains four new flags (`--encode-fragment`,
`--decode-fragment`, `--encode-xmldsig`, `--decode-xmldsig`) on Namespaces
whose schema defines the corresponding libcbv2g root. On SAP and DIN those
flags are absent from `--help` and rejected with a Namespace-specific error
when invoked.
"""

from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from EXIProcessor import EXIProcessor, Namespace  # noqa: E402

BUILD = REPO_ROOT / "build"


def _run(args, **kw):
    return subprocess.run(
        [str(a) for a in args], capture_output=True, text=True, timeout=10, **kw
    )


# --- --help advertises new flags on Fragment-capable Namespaces ---------------

# Only Namespaces whose libcbv2g header defines a `<prefix>_exiFragment` /
# `<prefix>_xmldsigFragment` root carry the four new flags. ISO20_WPT and
# ISO20_ACDP schemas define only `exiDocument`, so their CLI binaries match
# SAP/DIN — Document-only. The Python wrapper reflects the same gating via
# `hasattr(p, "encode_fragment")` per ADR-0014.
_FRAGMENT_CAPABLE_BINARIES = [
    "ISO2Processor",
    "ISO20CommonProcessor",
    "ISO20ACProcessor",
    "ISO20DCProcessor",
]
_DOCUMENT_ONLY_BINARIES = [
    "SupportedAppProtocolProcessor",
    "DINProcessor",
    "ISO20WPTProcessor",
    "ISO20ACDPProcessor",
]


@pytest.mark.parametrize("binary_name", _FRAGMENT_CAPABLE_BINARIES)
def test_fragment_capable_help_lists_all_new_flags(binary_name):
    result = _run([BUILD / binary_name, "-h"])
    assert result.returncode == 0, result.stderr
    for f in ("--encode-fragment", "--decode-fragment",
              "--encode-xmldsig",  "--decode-xmldsig"):
        assert f in result.stdout, f"{binary_name} --help should list {f}"


# --- --help on Namespaces without Fragment / XmldsigFragment roots ------------

_NEW_FLAGS = (
    "--encode-fragment",
    "--decode-fragment",
    "--encode-xmldsig",
    "--decode-xmldsig",
)


@pytest.mark.parametrize("binary_name", _DOCUMENT_ONLY_BINARIES)
def test_namespace_without_fragment_root_help_hides_new_flags(binary_name):
    result = _run([BUILD / binary_name, "-h"])
    assert result.returncode == 0, result.stderr
    for f in _NEW_FLAGS:
        assert f not in result.stdout, f"{binary_name} --help should not list {f}"
    # Existing two-mode flags must still be advertised.
    assert "--encode" in result.stdout
    assert "--decode" in result.stdout


# --- Unsupported root flag exits non-zero with Namespace-specific error -------

@pytest.mark.parametrize("binary_name,namespace,flag", [
    ("SupportedAppProtocolProcessor", "SAP", "--encode-fragment"),
    ("DINProcessor",                  "DIN", "--decode-xmldsig"),
])
def test_unsupported_root_flag_exits_with_namespace_error(binary_name, namespace, flag):
    result = _run([BUILD / binary_name, flag, "cmd"])
    assert result.returncode != 0
    assert flag in result.stderr
    assert f"Namespace={namespace}" in result.stderr


# --- CLI round-trip smoke tests for Fragment / XmldsigFragment ----------------

def _cli_roundtrip(binary, encode_flag, decode_flag, payload, namespace, tmp_path):
    """Drive the CLI through a single Fragment-root encode/decode cycle.

    Builds the known-good hex via the Python wrapper (which shares the same
    extern "C" entry point the CLI dispatches to), so a CLI regression is
    caught as a hex mismatch rather than as schema disagreement with the
    wrapper.
    """
    original_hex = (
        EXIProcessor(Namespace[namespace]).__getattribute__(
            "encode_fragment" if encode_flag.endswith("fragment")
            else "encode_xmldsig"
        )(payload).hex()
    )

    hex_in = tmp_path / "in.hex"
    hex_in.write_text(original_hex)
    json_mid = tmp_path / "mid.json"
    hex_out = tmp_path / "out.hex"

    decoded = _run([binary, decode_flag, "-i", str(hex_in), "-o", str(json_mid)])
    assert decoded.returncode == 0, decoded.stderr
    json_mid.read_text()  # parseable JSON

    encoded = _run([binary, encode_flag, "-i", str(json_mid), "-o", str(hex_out)])
    assert encoded.returncode == 0, encoded.stderr
    assert hex_out.read_text().strip() == original_hex


def test_iso2_fragment_roundtrip_via_cli(tmp_path):
    payload = {
        "DHpublickey": {
            "Id": {"charactersLen": 2, "characters": [ord("k"), ord("1")]},
            "CONTENT": {"bytesLen": 4, "bytes": [0xDE, 0xAD, 0xBE, 0xEF]},
        }
    }
    _cli_roundtrip(
        BUILD / "ISO2Processor",
        "--encode-fragment", "--decode-fragment",
        payload, "ISO2", tmp_path,
    )


def test_iso20_common_xmldsig_roundtrip_via_cli(tmp_path):
    fixture = (
        REPO_ROOT / "tests" / "integration" / "fixtures"
        / "ISO20_COMMON" / "SignedInfo.json"
    )
    payload = json.loads(fixture.read_text())
    _cli_roundtrip(
        BUILD / "ISO20CommonProcessor",
        "--encode-xmldsig", "--decode-xmldsig",
        payload, "ISO20_COMMON", tmp_path,
    )
