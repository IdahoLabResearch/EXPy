"""Schema-driven Document roundtrip sweep for the ISO20_WPT Namespace.

The WPT schema only defines an `exiDocument` root (no Fragment /
xmldsigFragment), so the document union mixes WPT message types and the
W3C XML-DSig type tree directly. Each union member is a concrete Document
root.
"""

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))
sys.path.insert(0, str(REPO_ROOT / "tools"))

import expy.v2gjson.iso20_wpt as v2gjson_iso20_wpt  # noqa: E402

from codegen.fixture_emitter import (  # noqa: E402
    emit_fragment_scenarios,
    harvest_enum_names,
)
from codegen.iso20_wpt_choices import CHOICES as ISO20_WPT_CHOICES  # noqa: E402
from codegen.parser import parse_header  # noqa: E402
from conftest import assert_roundtrip  # noqa: E402


ISO20_WPT_HEADER = (
    REPO_ROOT / "extern" / "libcbv2g" / "include" / "cbv2g" / "iso_20"
    / "iso20_WPT_Datatypes.h"
)

_HEADER_TEXT = ISO20_WPT_HEADER.read_text()
_SPECS = {s.name: s for s in parse_header(_HEADER_TEXT)}
_ENUMS = harvest_enum_names(_HEADER_TEXT)

ISO20_WPT_OVERRIDES: dict = {}


def _discover_document_members():
    doc_spec = _SPECS["iso20_wpt_exiDocument"]
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
            overrides=ISO20_WPT_OVERRIDES,
            v2gjson=v2gjson_iso20_wpt,
            namespace_prefix="iso20_wpt_",
            choices=ISO20_WPT_CHOICES,
        )


_SCENARIOS = list(_scenarios())

_LOOP_GRAMMAR_XFAILS: frozenset[str] = frozenset()


def _param(scenario):
    sid = scenario[0]
    if sid in _LOOP_GRAMMAR_XFAILS:
        return pytest.param(
            scenario,
            id=sid,
            marks=pytest.mark.xfail(
                reason="libcbv2g WPT TxSpecData/RxSpecData loop-exit grammar bug, tracked in #26",
                strict=True,
            ),
        )
    return pytest.param(scenario, id=sid)


@pytest.mark.parametrize("scenario", [_param(s) for s in _SCENARIOS])
def test_iso20_wpt_generated_document_roundtrip(scenario):
    _scenario_id, payload = scenario
    assert_roundtrip("ISO20_WPT", payload)
