"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

import json
from pathlib import Path

import pytest

from conftest import assert_roundtrip

# ISO2 Document scenarios are produced by the schema-driven generator in
# `test_iso2_generated_roundtrip.py`. The hand-written Fragment / Xmldsig
# fixtures live here until #14 / #15 land their generator coverage.
FIXTURES_DIR = Path(__file__).parent / "fixtures" / "ISO2"


def _discover_fragments():
    return sorted(FIXTURES_DIR.glob("Fragment_*.json"))


def _discover_xmldsigs():
    return sorted(FIXTURES_DIR.glob("Xmldsig_*.json"))


@pytest.mark.parametrize("fixture_path", _discover_fragments(), ids=lambda p: p.stem)
def test_iso2_fragment_roundtrip(fixture_path):
    payload = json.loads(fixture_path.read_text())
    assert_roundtrip("ISO2", payload, kind="fragment")


@pytest.mark.parametrize("fixture_path", _discover_xmldsigs(), ids=lambda p: p.stem)
def test_iso2_xmldsig_roundtrip(fixture_path):
    payload = json.loads(fixture_path.read_text())
    assert_roundtrip("ISO2", payload, kind="xmldsig")
