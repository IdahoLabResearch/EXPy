"""Schema-driven Document roundtrip sweep for the ISO20_ACDP Namespace.

The ACDP schema only defines an `exiDocument` root (no Fragment /
xmldsigFragment), so the document union mixes ACDP message types and the
W3C XML-DSig type tree directly. Each union member is a concrete Document
root.
"""

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))
sys.path.insert(0, str(REPO_ROOT / "tools"))

import V2Gjson.iso20_acdp as v2gjson_iso20_acdp  # noqa: E402

from codegen.fixture_emitter import (  # noqa: E402
    emit_fragment_scenarios,
    harvest_enum_names,
)
from codegen.iso20_acdp_choices import CHOICES as ISO20_ACDP_CHOICES  # noqa: E402
from codegen.parser import parse_header  # noqa: E402
from conftest import assert_roundtrip  # noqa: E402


ISO20_ACDP_HEADER = (
    REPO_ROOT / "extern" / "libcbv2g" / "include" / "cbv2g" / "iso_20"
    / "iso20_ACDP_Datatypes.h"
)

_HEADER_TEXT = ISO20_ACDP_HEADER.read_text()
_SPECS = {s.name: s for s in parse_header(_HEADER_TEXT)}
_ENUMS = harvest_enum_names(_HEADER_TEXT)

ISO20_ACDP_OVERRIDES: dict = {}


def _discover_document_members():
    doc_spec = _SPECS["iso20_acdp_exiDocument"]
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
            overrides=ISO20_ACDP_OVERRIDES,
            v2gjson=v2gjson_iso20_acdp,
            namespace_prefix="iso20_acdp_",
            choices=ISO20_ACDP_CHOICES,
        )


_SCENARIOS = list(_scenarios())

@pytest.mark.parametrize(
    "scenario", [pytest.param(s, id=s[0]) for s in _SCENARIOS]
)
def test_iso20_acdp_generated_document_roundtrip(scenario):
    _scenario_id, payload = scenario
    assert_roundtrip("ISO20_ACDP", payload)
