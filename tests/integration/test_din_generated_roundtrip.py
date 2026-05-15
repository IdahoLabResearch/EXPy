"""Integration roundtrip over generator-produced DIN Document payloads.

Enumerates `din_BodyType`'s inner union and roundtrips each top-level Document
at minimal + maximal. Per-XSD-choice-branch coverage is deferred to #18.
"""

import json
import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))
sys.path.insert(0, str(REPO_ROOT / "tools"))

import V2Gjson.din as v2gjson_din  # noqa: E402

from codegen.din_choices import CHOICES as DIN_CHOICES  # noqa: E402
from codegen.fixture_emitter import (  # noqa: E402
    emit_document_scenarios,
    harvest_enum_names,
)
from codegen.parser import parse_header  # noqa: E402
from conftest import assert_roundtrip  # noqa: E402


DIN_HEADER = REPO_ROOT / "extern" / "libcbv2g" / "include" / "cbv2g" / "din" / "din_msgDefDatatypes.h"
HEADER_FIXTURE = Path(__file__).parent / "fixtures" / "DIN" / "DIN_Header.json"

_HEADER_TEXT = DIN_HEADER.read_text()
_SPECS = {s.name: s for s in parse_header(_HEADER_TEXT)}
_ENUMS = harvest_enum_names(_HEADER_TEXT)

# Seed-overrides for fields the per-kind default cannot satisfy. Keys are
# `(struct-type, field-name)`. EVCCID is xsd:length-6 in DIN (the parser only
# sees the buffer cap, not the exact-length constraint).
DIN_OVERRIDES = {
    ("din_SessionSetupReqType", "EVCCID"): {"bytes": [0] * 6, "bytesLen": 6},
}


def _discover_body_members():
    body_spec = _SPECS["din_BodyType"]
    out = []
    for f in body_spec.fields:
        if f.kind != "struct":
            continue
        if f.name == "BodyElement":
            continue
        out.append((f.c_type, f.name))
    return out


_DOCUMENT_TYPES = _discover_body_members()


def _scenarios():
    for type_name, element in _DOCUMENT_TYPES:
        yield from emit_document_scenarios(
            type_name,
            element_name=element,
            specs=_SPECS,
            enum_names=_ENUMS,
            overrides=DIN_OVERRIDES,
            v2gjson=v2gjson_din,
            namespace_prefix="din_",
            choices=DIN_CHOICES,
        )


_SCENARIOS = list(_scenarios())

# DIN Documents whose XSD-choice tree is not yet covered by the manifest.
# Empty: every choice-bearing DIN Document type is in din_choices.CHOICES.
_CHOICE_BEARING_SCENARIO_IDS: frozenset[str] = frozenset()


def _param(scenario):
    sid = scenario[0]
    if sid in _CHOICE_BEARING_SCENARIO_IDS:
        return pytest.param(
            scenario,
            id=sid,
            marks=pytest.mark.xfail(
                reason="XSD-choice support deferred to #18",
                strict=True,
            ),
        )
    return pytest.param(scenario, id=sid)


@pytest.fixture(scope="module")
def header():
    return json.loads(HEADER_FIXTURE.read_text())


@pytest.mark.parametrize("scenario", [_param(s) for s in _SCENARIOS])
def test_din_generated_document_roundtrip(scenario, header):
    _scenario_id, payload = scenario
    payload = {**payload, **header}
    assert_roundtrip("DIN", payload)
