"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

import json
from pathlib import Path

import pytest

from conftest import assert_roundtrip

# ISO2 Document and Fragment scenarios are produced by the schema-driven
# generator in `test_iso2_generated_roundtrip.py` and
# `test_iso2_generated_fragment_roundtrip.py`. The hand-written Xmldsig
# fixture lives here until #15 lands its generator coverage.
FIXTURES_DIR = Path(__file__).parent / "fixtures" / "ISO2"


def _discover_xmldsigs():
    return sorted(FIXTURES_DIR.glob("Xmldsig_*.json"))


@pytest.mark.parametrize("fixture_path", _discover_xmldsigs(), ids=lambda p: p.stem)
def test_iso2_xmldsig_roundtrip(fixture_path):
    payload = json.loads(fixture_path.read_text())
    assert_roundtrip("ISO2", payload, kind="xmldsig")
