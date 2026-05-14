"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

import json
from pathlib import Path

import pytest

from conftest import assert_roundtrip

FIXTURES_DIR = Path(__file__).parent / "fixtures" / "AppHandshake"


@pytest.mark.parametrize(
    "fixture_path",
    sorted(FIXTURES_DIR.glob("*.json")),
    ids=lambda p: p.stem,
)
def test_app_handshake_roundtrip(fixture_path):
    payload = json.loads(fixture_path.read_text())
    assert_roundtrip("HANDSHAKE", payload)
