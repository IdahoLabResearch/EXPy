"""End-to-end: CLI parses a header file and writes a marshalers .cpp."""

import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]

HEADER = """\
struct din_RelativeTimeIntervalType {
    uint32_t start;
    uint32_t duration;
    unsigned int duration_isUsed:1;
};
"""


def _run(args, cwd):
    env = {"PYTHONPATH": str(REPO_ROOT / "tools")}
    return subprocess.run(
        [sys.executable, "-m", "codegen", *args],
        cwd=cwd,
        capture_output=True,
        text=True,
        env={**env, "PATH": "/usr/bin:/bin"},
        check=False,
    )


def test_cli_writes_marshalers_for_one_struct(tmp_path):
    header = tmp_path / "din_msgDefDatatypes.h"
    header.write_text(HEADER)
    out = tmp_path / "out.cpp"

    proc = _run(["--header", str(header), "--out", str(out)], tmp_path)
    assert proc.returncode == 0, proc.stderr

    text = out.read_text()
    assert "json getJson_RelativeTimeIntervalType(" in text
    assert "struct din_RelativeTimeIntervalType getDoc_RelativeTimeIntervalType(" in text
    assert "if (RelativeTimeIntervalTypeDoc.duration_isUsed) {" in text
    assert 'if (RelativeTimeIntervalTypeJson.contains("duration")) {' in text
