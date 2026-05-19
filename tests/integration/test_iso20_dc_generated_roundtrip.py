"""Schema-driven Document roundtrip sweep for the ISO20_DC Namespace.

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

ISO20_DC_OVERRIDES: dict = {}


def _discover_document_members():
    doc_spec = _SPECS["iso20_dc_exiDocument"]
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
            overrides=ISO20_DC_OVERRIDES,
            v2gjson=v2gjson_iso20_dc,
            namespace_prefix="iso20_dc_",
            choices=ISO20_DC_CHOICES,
        )


_SCENARIOS = list(_scenarios())

# Same iso20 PGPData grammar bugs that affect ISO20_COMMON apply here —
# tracked in #25.
_NESTED_CHOICE_XFAILS: frozenset[str] = frozenset({
    "KeyInfo__choice_PGPData",
    "PGPData__maximal",
})


def _param(scenario):
    sid = scenario[0]
    if sid in _NESTED_CHOICE_XFAILS:
        return pytest.param(
            scenario,
            id=sid,
            marks=pytest.mark.xfail(
                reason="iso20 PGPData grammar rewrite deferred to #25",
                strict=True,
            ),
        )
    return pytest.param(scenario, id=sid)


@pytest.mark.parametrize("scenario", [_param(s) for s in _SCENARIOS])
def test_iso20_dc_generated_document_roundtrip(scenario):
    _scenario_id, payload = scenario
    assert_roundtrip("ISO20_DC", payload)
