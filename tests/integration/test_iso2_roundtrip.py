"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

import json
from pathlib import Path

import pytest

from conftest import assert_roundtrip

FIXTURES_DIR = Path(__file__).parent / "fixtures" / "ISO2"
HEADER_FIXTURE = FIXTURES_DIR / "ISO2_Header.json"


def _discover_documents():
    return sorted(
        p for p in FIXTURES_DIR.glob("*.json")
        if "Header" not in p.name and not p.name.startswith("Fragment_") and not p.name.startswith("Xmldsig_")
    )


def _discover_fragments():
    return sorted(FIXTURES_DIR.glob("Fragment_*.json"))


def _discover_xmldsigs():
    return sorted(FIXTURES_DIR.glob("Xmldsig_*.json"))


@pytest.fixture(scope="module")
def header():
    return json.loads(HEADER_FIXTURE.read_text())


@pytest.mark.parametrize("fixture_path", _discover_documents(), ids=lambda p: p.stem)
def test_iso2_document_roundtrip(fixture_path, header):
    payload = json.loads(fixture_path.read_text())
    payload.update(header)
    assert_roundtrip("ISO2", payload)


@pytest.mark.parametrize("fixture_path", _discover_fragments(), ids=lambda p: p.stem)
def test_iso2_fragment_roundtrip(fixture_path):
    payload = json.loads(fixture_path.read_text())
    assert_roundtrip("ISO2", payload, kind="fragment")


@pytest.mark.parametrize("fixture_path", _discover_xmldsigs(), ids=lambda p: p.stem)
def test_iso2_xmldsig_roundtrip(fixture_path):
    payload = json.loads(fixture_path.read_text())
    assert_roundtrip("ISO2", payload, kind="xmldsig")
