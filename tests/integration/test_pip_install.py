"""End-to-end packaging tests per ADR-0013.

Builds a wheel from the repo, installs it into a fresh venv with no source
tree on ``sys.path``, and exercises the installed surface. Catches regressions
the source-tree tests can't see: missing package data, broken ``[project.scripts]``
entries, ``_resolve_so_path`` not finding the bundled ``.so`` files.

The session-scoped ``installed_venv`` fixture pays the wheel-build/install cost
exactly once per pytest invocation; individual tests run cheaply against the
resulting interpreter. Marked ``slow`` so it can be deselected with
``-m 'not slow'`` during tight inner-loop work.
"""

from __future__ import annotations

import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]


pytestmark = pytest.mark.slow


@dataclass(frozen=True)
class InstalledVenv:
    """A fresh venv with the wheel installed and no repo on sys.path."""

    python: Path
    bin_dir: Path


@pytest.fixture(scope="session")
def installed_venv(tmp_path_factory) -> InstalledVenv:
    workdir = tmp_path_factory.mktemp("expy_install")
    wheelhouse = workdir / "wheelhouse"
    venv_dir = workdir / "venv"

    # Build a wheel from the repo using the *outer* interpreter's pip. Build
    # isolation is on, so pyproject.toml's build-system.requires is honored.
    subprocess.run(
        [
            sys.executable, "-m", "pip", "wheel",
            "--no-deps",
            "--wheel-dir", str(wheelhouse),
            str(REPO_ROOT),
        ],
        check=True,
        capture_output=True,
        text=True,
    )
    wheels = sorted(wheelhouse.glob("expy-*.whl"))
    assert wheels, f"no expy wheel produced in {wheelhouse}"
    wheel_path = wheels[-1]

    # Fresh venv — system-site-packages off, no repo on sys.path.
    subprocess.run(
        [sys.executable, "-m", "venv", str(venv_dir)],
        check=True,
        capture_output=True,
        text=True,
    )
    bin_dir = venv_dir / "bin"
    venv_python = bin_dir / "python"

    subprocess.run(
        [str(venv_python), "-m", "pip", "install", "--quiet", str(wheel_path)],
        check=True,
        capture_output=True,
        text=True,
    )

    return InstalledVenv(python=venv_python, bin_dir=bin_dir)


def _run(venv: InstalledVenv, code: str, *, cwd: Path) -> subprocess.CompletedProcess:
    """Run a snippet under the installed venv's Python from `cwd`.

    `cwd` is set to a directory *outside* the repo so the source-tree ``expy/``
    package never shadows the installed copy on ``sys.path``.
    """
    return subprocess.run(
        [str(venv.python), "-c", code],
        cwd=str(cwd),
        capture_output=True,
        text=True,
        timeout=30,
    )


@pytest.fixture(scope="session")
def outside_repo(tmp_path_factory) -> Path:
    return tmp_path_factory.mktemp("expy_runcwd")


def test_wheel_installs_and_imports_expy(installed_venv, outside_repo):
    result = _run(installed_venv, "import expy; print(expy.__name__)", cwd=outside_repo)
    assert result.returncode == 0, (
        f"`import expy` failed in installed venv\nstdout: {result.stdout}\nstderr: {result.stderr}"
    )
    assert result.stdout.strip() == "expy"


def test_installed_package_exposes_version(installed_venv, outside_repo):
    """Per ADR-0012, ``__version__`` is re-exported at the ``expy`` package
    level from the single canonical source (``expy/_version.py``)."""
    result = _run(installed_venv, "from expy import __version__; print(__version__)", cwd=outside_repo)
    assert result.returncode == 0, result.stderr
    assert result.stdout.strip() == "1.0.0"


# All eight Namespaces per ADR-0002. The wheel must ship one ``lib-*Processor.so``
# per Namespace under ``expy/v2gjson/`` as package data (ADR-0013) so that
# ``EXIProcessor(Namespace.X)`` constructs without a repo-relative ``./build/`` path.
_ALL_NAMESPACE_NAMES = (
    "SAP",
    "DIN",
    "ISO2",
    "ISO20_COMMON",
    "ISO20_AC",
    "ISO20_DC",
    "ISO20_WPT",
    "ISO20_ACDP",
)


@pytest.mark.parametrize("namespace_name", _ALL_NAMESPACE_NAMES)
def test_processor_constructs_post_install(installed_venv, outside_repo, namespace_name):
    """Every Namespace's ``.so`` must be reachable from the installed package."""
    code = (
        "from expy import EXIProcessor, Namespace; "
        f"EXIProcessor(Namespace.{namespace_name}); "
        "print('ok')"
    )
    result = _run(installed_venv, code, cwd=outside_repo)
    assert result.returncode == 0, (
        f"EXIProcessor(Namespace.{namespace_name}) failed post-install\n"
        f"stdout: {result.stdout}\nstderr: {result.stderr}"
    )
    assert result.stdout.strip() == "ok"


def test_decode_known_din_hex_post_install(installed_venv, outside_repo):
    """End-to-end smoke per ADR-0013 acceptance criteria: a known DIN
    PreChargeReq decodes to its expected message-name key. Exercises the full
    install path — package import, ``.so`` resolution via package data, the
    ctypes decode call. Mirrors example.py's first scenario."""
    code = (
        "from expy import EXIProcessor, Namespace; "
        "p = EXIProcessor(Namespace.DIN); "
        "out = p.decode(bytes.fromhex('809a021050908c0c0c0c0c51514002808142807c0c0c0000')); "
        "print(list(out['Body'])[0])"
    )
    result = _run(installed_venv, code, cwd=outside_repo)
    assert result.returncode == 0, result.stderr
    assert result.stdout.strip() == "PreChargeReq"


# CLI binary names per ADR-0012 / ADR-0014: one console-script per Namespace.
_CLI_BINARIES = (
    "SupportedAppProtocolProcessor",
    "DINProcessor",
    "ISO2Processor",
    "ISO20CommonProcessor",
    "ISO20ACProcessor",
    "ISO20DCProcessor",
    "ISO20WPTProcessor",
    "ISO20ACDPProcessor",
)


@pytest.mark.parametrize("binary_name", _CLI_BINARIES)
def test_cli_binary_on_path_post_install(installed_venv, outside_repo, binary_name):
    """Each per-Namespace CLI is installed as a console script in the venv's
    ``bin/`` per ADR-0013 (CLI delivery via project.scripts)."""
    binary = installed_venv.bin_dir / binary_name
    assert binary.exists(), f"console script missing post-install: {binary}"
    result = subprocess.run(
        [str(binary), "-h"],
        cwd=str(outside_repo),
        capture_output=True, text=True, timeout=10,
    )
    assert result.returncode == 0, (
        f"{binary_name} -h failed\nstdout: {result.stdout}\nstderr: {result.stderr}"
    )
    assert "Encode and decode V2G EXI Documents" in result.stdout


def test_cli_binary_decode_encode_roundtrip_post_install(installed_venv, outside_repo, tmp_path):
    """Drive a full --decode → --encode loop through the installed DIN binary
    and assert the encoded hex matches the original. Confirms the CLI wrapper,
    bundled binary, and ctypes path inside the same wheel all line up."""
    binary = installed_venv.bin_dir / "DINProcessor"

    original_hex = "809a021050908c0c0c0c0c51514002808142807c0c0c0000"
    hex_in = tmp_path / "in.hex"
    hex_in.write_text(original_hex)
    json_mid = tmp_path / "mid.json"
    hex_out = tmp_path / "out.hex"

    decoded = subprocess.run(
        [str(binary), "--decode", "-i", str(hex_in), "-o", str(json_mid)],
        cwd=str(outside_repo), capture_output=True, text=True, timeout=10,
    )
    assert decoded.returncode == 0, decoded.stderr
    assert json_mid.exists()

    encoded = subprocess.run(
        [str(binary), "--encode", "-i", str(json_mid), "-o", str(hex_out)],
        cwd=str(outside_repo), capture_output=True, text=True, timeout=10,
    )
    assert encoded.returncode == 0, encoded.stderr
    assert hex_out.read_text().strip() == original_hex
