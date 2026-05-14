"""Integration roundtrip over payloads produced by the schema-driven generator.

The generator (`tools/codegen/fixture_emitter.py`) parses the libcbv2g headers
into TypeSpecs, walks them with per-kind defaults plus a small seed-override
table for length-constrained fields, and yields min/max scenarios per type.
"""

import json
import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))
sys.path.insert(0, str(REPO_ROOT / "tools"))

import V2Gjson.iso2 as v2gjson_iso2  # noqa: E402

from codegen.fixture_emitter import (  # noqa: E402
    emit_document_scenarios,
    harvest_enum_names,
)
from codegen.parser import parse_header  # noqa: E402
from conftest import assert_roundtrip  # noqa: E402


ISO2_HEADER = REPO_ROOT / "extern" / "libcbv2g" / "include" / "cbv2g" / "iso_2" / "iso2_msgDefDatatypes.h"
HEADER_FIXTURE = Path(__file__).parent / "fixtures" / "ISO2" / "ISO2_Header.json"

_HEADER_TEXT = ISO2_HEADER.read_text()
_SPECS = {s.name: s for s in parse_header(_HEADER_TEXT)}
_ENUMS = harvest_enum_names(_HEADER_TEXT)

# Seed-overrides for fields the per-kind default cannot satisfy. Keys are
# `(struct-type, field-name)`. The C parser cannot tell exact-length bytes
# (xsd:length) from max-length bytes (xsd:maxLength); exact-length entries
# need explicit values here.
ISO2_OVERRIDES = {
    ("iso2_SessionSetupReqType", "EVCCID"): {"bytes": [0] * 6, "bytesLen": 6},
}

# MVP slice for #12: one type exercising the seed-override path
# (SessionSetupReq, exact-length EVCCID) plus one exercising enum resolution
# via V2Gjson (AuthorizationRes, two enum-typed scalars). Broader coverage
# lands in #13.
_DEMO_TYPES = [
    ("iso2_SessionSetupReqType", "SessionSetupReq"),
    ("iso2_AuthorizationResType", "AuthorizationRes"),
]


def _scenarios():
    for type_name, element in _DEMO_TYPES:
        yield from emit_document_scenarios(
            type_name,
            element_name=element,
            specs=_SPECS,
            enum_names=_ENUMS,
            overrides=ISO2_OVERRIDES,
            v2gjson=v2gjson_iso2,
            namespace_prefix="iso2_",
        )


_SCENARIOS = list(_scenarios())


@pytest.fixture(scope="module")
def header():
    return json.loads(HEADER_FIXTURE.read_text())


@pytest.mark.parametrize("scenario", _SCENARIOS, ids=lambda s: s[0])
def test_iso2_generated_document_roundtrip(scenario, header):
    _scenario_id, payload = scenario
    payload = {**payload, **header}
    assert_roundtrip("ISO2", payload)
