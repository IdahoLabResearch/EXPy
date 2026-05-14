"""Integration roundtrip over generator-produced ISO2 Fragment payloads.

Enumerates the members of the `iso2_exiFragment` union and roundtrips each at
minimal + maximal. Per-XSD-choice-branch coverage is deferred to #18.
"""

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))
sys.path.insert(0, str(REPO_ROOT / "tools"))

import V2Gjson.iso2 as v2gjson_iso2  # noqa: E402

from codegen.fixture_emitter import (  # noqa: E402
    emit_fragment_scenarios,
    harvest_enum_names,
)
from codegen.iso2_choices import CHOICES as ISO2_CHOICES  # noqa: E402
from codegen.parser import parse_header  # noqa: E402
from conftest import assert_roundtrip  # noqa: E402


ISO2_HEADER = REPO_ROOT / "extern" / "libcbv2g" / "include" / "cbv2g" / "iso_2" / "iso2_msgDefDatatypes.h"

_HEADER_TEXT = ISO2_HEADER.read_text()
_SPECS = {s.name: s for s in parse_header(_HEADER_TEXT)}
_ENUMS = harvest_enum_names(_HEADER_TEXT)

# Seed-overrides for fields the per-kind default cannot satisfy. Mirrors the
# Document-side overrides — exact-length bytes the parser cannot distinguish
# from max-length bytes need explicit values here.
ISO2_FRAGMENT_OVERRIDES = {
    ("iso2_AuthorizationReqType", "GenChallenge"): {"bytes": [0] * 16, "bytesLen": 16},
}


def _discover_fragment_members():
    """Pull (struct-type, element-name) for each member of `iso2_exiFragment`'s
    inner union, in declaration order.
    """
    spec = _SPECS["iso2_exiFragment"]
    return [(f.c_type, f.name) for f in spec.fields if f.kind == "struct"]


_FRAGMENT_TYPES = _discover_fragment_members()


def _scenarios():
    for type_name, element in _FRAGMENT_TYPES:
        yield from emit_fragment_scenarios(
            type_name,
            element_name=element,
            specs=_SPECS,
            enum_names=_ENUMS,
            overrides=ISO2_FRAGMENT_OVERRIDES,
            v2gjson=v2gjson_iso2,
            namespace_prefix="iso2_",
            choices=ISO2_CHOICES,
        )


_SCENARIOS = list(_scenarios())

# Fragment roots that internally contain an XSD-choice not yet covered by the
# manifest (still deferred to #18). The SalesTariff choice is in the manifest;
# SignedInfo's inner TransformType choice is not yet — that lands in a later
# slice of #18.
_CHOICE_BEARING_SCENARIO_IDS = frozenset({
    # SignedInfo.Reference[].Transforms.Transform — TransformType is a choice
    # of {ANY, XPath}; only the maximal variant sets both branches.
    "SignedInfo__maximal",
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


@pytest.mark.parametrize("scenario", [_param(s) for s in _SCENARIOS])
def test_iso2_generated_fragment_roundtrip(scenario):
    _scenario_id, payload = scenario
    assert_roundtrip("ISO2", payload, kind="fragment")
