"""Schema-driven Document roundtrip sweep for the ISO20_AC Namespace.

The ISO-20 `exiDocument` is a union over message types directly — there is
no intermediate `V2G_Message`/`Body` wrapper. Each union member is a
concrete Document root with its own `Header`.
"""

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))
sys.path.insert(0, str(REPO_ROOT / "tools"))

import expy.v2gjson.iso20_ac as v2gjson_iso20_ac  # noqa: E402

from codegen.fixture_emitter import (  # noqa: E402
    emit_fragment_scenarios,
    harvest_enum_names,
)
from codegen.iso20_ac_choices import CHOICES as ISO20_AC_CHOICES  # noqa: E402
from codegen.parser import parse_header  # noqa: E402
from conftest import assert_roundtrip  # noqa: E402


ISO20_AC_HEADER = (
    REPO_ROOT / "extern" / "libcbv2g" / "include" / "cbv2g" / "iso_20"
    / "iso20_AC_Datatypes.h"
)

_HEADER_TEXT = ISO20_AC_HEADER.read_text()
_SPECS = {s.name: s for s in parse_header(_HEADER_TEXT)}
_ENUMS = harvest_enum_names(_HEADER_TEXT)

ISO20_AC_OVERRIDES: dict = {}


def _discover_document_members():
    doc_spec = _SPECS["iso20_ac_exiDocument"]
    out = []
    for f in doc_spec.fields:
        if f.kind != "struct":
            continue
        out.append((f.c_type, f.name))
    return out


_DOCUMENT_TYPES = _discover_document_members()


def _scenarios():
    for type_name, element in _DOCUMENT_TYPES:
        yield from emit_fragment_scenarios(
            type_name,
            element_name=element,
            specs=_SPECS,
            enum_names=_ENUMS,
            overrides=ISO20_AC_OVERRIDES,
            v2gjson=v2gjson_iso20_ac,
            namespace_prefix="iso20_ac_",
            choices=ISO20_AC_CHOICES,
        )


_SCENARIOS = list(_scenarios())

@pytest.mark.parametrize(
    "scenario", [pytest.param(s, id=s[0]) for s in _SCENARIOS]
)
def test_iso20_ac_generated_document_roundtrip(scenario):
    _scenario_id, payload = scenario
    assert_roundtrip("ISO20_AC", payload)
