"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

import json
from pathlib import Path

import pytest

from conftest import assert_roundtrip

FIXTURES_DIR = Path(__file__).parent / "fixtures" / "SAP"


@pytest.mark.parametrize(
    "fixture_path",
    sorted(FIXTURES_DIR.glob("*.json")),
    ids=lambda p: p.stem,
)
def test_sap_roundtrip(fixture_path):
    payload = json.loads(fixture_path.read_text())
    assert_roundtrip("SAP", payload)
