"""Integration roundtrip over payloads produced by the schema-driven generator.

The generator (`tools/codegen/fixture_emitter.py`) parses the libcbv2g headers
into TypeSpecs, walks them with per-kind defaults plus a small seed-override
table for length-constrained fields, and yields min/max scenarios per type.
"""

import json
import re
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


def _discover_body_members():
    """Pull (struct-type, element-name) for each member of `iso2_BodyType`'s
    inner union, in declaration order. `BodyElement` (the abstract base) is
    excluded — it isn't a concrete Document.
    """
    body_spec = _SPECS["iso2_BodyType"]
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
            overrides=ISO2_OVERRIDES,
            v2gjson=v2gjson_iso2,
            namespace_prefix="iso2_",
        )


_SCENARIOS = list(_scenarios())

# Documents that contain an XSD-choice the generator cannot honor without
# per-choice-branch support (deferred to #18). The naive minimal-with-no-branch
# and maximal-with-all-branches scenarios both fail to round-trip cleanly:
# minimal leaves uninitialized choice memory that the encoder still emits;
# maximal sets multiple choice members and the encoder only keeps one.
_CHOICE_BEARING_SCENARIO_IDS = frozenset({
    # ChargeParameter* — choice of {AC,DC,EV} ChargeParameter.
    "ChargeParameterDiscoveryReq__minimal",
    "ChargeParameterDiscoveryReq__maximal",
    "ChargeParameterDiscoveryRes__minimal",
    "ChargeParameterDiscoveryRes__maximal",
    # PowerDeliveryReq.EVPowerDeliveryParameter — choice of {DC,EV}.
    "PowerDeliveryReq__maximal",
    # *Res types embedding EVSEStatusType — choice of {AC_EVSEStatus, DC_EVSEStatus, EVSEStatus}.
    "MeteringReceiptRes__minimal",
    "MeteringReceiptRes__maximal",
    "PowerDeliveryRes__minimal",
    "PowerDeliveryRes__maximal",
    # ServiceDetailRes.ServiceParameterList.ParameterSet.Parameter — choice across value kinds.
    "ServiceDetailRes__maximal",
})


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
def test_iso2_generated_document_roundtrip(scenario, header):
    _scenario_id, payload = scenario
    payload = {**payload, **header}
    assert_roundtrip("ISO2", payload)
