"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

import json
from pathlib import Path

import pytest

from conftest import assert_roundtrip

FIXTURES_DIR = Path(__file__).parent / "fixtures" / "DIN"
HEADER_FIXTURE = FIXTURES_DIR / "DIN_Header.json"


def _discover_packets():
    return sorted(p for p in FIXTURES_DIR.glob("*.json") if "Header" not in p.name)


@pytest.fixture(scope="module")
def header():
    return json.loads(HEADER_FIXTURE.read_text())


@pytest.mark.parametrize("fixture_path", _discover_packets(), ids=lambda p: p.stem)
def test_din_roundtrip(fixture_path, header):
    payload = json.loads(fixture_path.read_text())
    payload.update(header)
    assert_roundtrip("DIN", payload)
