"""Integration roundtrip over generator-produced ISO20_DC Fragment payloads.

Enumerates the members of the `iso20_dc_exiFragment` union and roundtrips each
at minimal + maximal (or per-XSD-choice-branch where the manifest covers it).
"""

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))
sys.path.insert(0, str(REPO_ROOT / "tools"))

import V2Gjson.iso20_dc as v2gjson_iso20_dc  # noqa: E402

from codegen.fixture_emitter import (  # noqa: E402
    emit_fragment_scenarios,
    harvest_enum_names,
)
from codegen.iso20_dc_choices import CHOICES as ISO20_DC_CHOICES  # noqa: E402
from codegen.parser import parse_header  # noqa: E402
from conftest import assert_roundtrip  # noqa: E402


ISO20_DC_HEADER = (
    REPO_ROOT / "extern" / "libcbv2g" / "include" / "cbv2g" / "iso_20"
    / "iso20_DC_Datatypes.h"
)

_HEADER_TEXT = ISO20_DC_HEADER.read_text()
_SPECS = {s.name: s for s in parse_header(_HEADER_TEXT)}
_ENUMS = harvest_enum_names(_HEADER_TEXT)

ISO20_DC_FRAGMENT_OVERRIDES: dict = {}


def _discover_fragment_members():
    spec = _SPECS["iso20_dc_exiFragment"]
    return [(f.c_type, f.name) for f in spec.fields if f.kind == "struct"]


_FRAGMENT_TYPES = _discover_fragment_members()


def _scenarios():
    for type_name, element in _FRAGMENT_TYPES:
        yield from emit_fragment_scenarios(
            type_name,
            element_name=element,
            specs=_SPECS,
            enum_names=_ENUMS,
            overrides=ISO20_DC_FRAGMENT_OVERRIDES,
            v2gjson=v2gjson_iso20_dc,
            namespace_prefix="iso20_dc_",
            choices=ISO20_DC_CHOICES,
        )


_SCENARIOS = list(_scenarios())


@pytest.mark.parametrize("scenario", [pytest.param(s, id=s[0]) for s in _SCENARIOS])
def test_iso20_dc_generated_fragment_roundtrip(scenario):
    _scenario_id, payload = scenario
    assert_roundtrip("ISO20_DC", payload, kind="fragment")
