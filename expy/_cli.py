"""Console-script launchers for the eight per-Namespace CLI binaries (ADR-0013).

Each ``[project.scripts]`` entry in ``pyproject.toml`` points at one of the
functions below. The function resolves the bundled binary inside the installed
``expy/_bin/`` directory and ``os.execv``s into it, so the running shell sees
the C++ process directly (signals, exit code, stdio all native — no Python
process lingers).
"""

from __future__ import annotations

import os
import sys
from pathlib import Path

_BIN_DIR = Path(__file__).resolve().parent / "_bin"


def _launch(binary_name: str) -> None:
    binary = _BIN_DIR / binary_name
    if not binary.exists():
        sys.stderr.write(
            f"expy: bundled binary missing: {binary}\n"
            "The wheel may have been installed without the native build step.\n"
        )
        sys.exit(127)
    os.execv(str(binary), [str(binary), *sys.argv[1:]])


def SupportedAppProtocolProcessor() -> None:
    _launch("SupportedAppProtocolProcessor")


def DINProcessor() -> None:
    _launch("DINProcessor")


def ISO2Processor() -> None:
    _launch("ISO2Processor")


def ISO20CommonProcessor() -> None:
    _launch("ISO20CommonProcessor")


def ISO20ACProcessor() -> None:
    _launch("ISO20ACProcessor")


def ISO20DCProcessor() -> None:
    _launch("ISO20DCProcessor")


def ISO20WPTProcessor() -> None:
    _launch("ISO20WPTProcessor")


def ISO20ACDPProcessor() -> None:
    _launch("ISO20ACDPProcessor")
