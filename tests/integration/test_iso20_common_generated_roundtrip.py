"""Schema-driven Document roundtrip sweep for the ISO20_COMMON Namespace.

ISO20's `exiDocument` is a union over message types directly — there is no
intermediate `V2G_Message`/`Body` wrapper as in ISO-2 / DIN. Each message
type owns its own `Header`. We therefore drive the generator with the
Fragment-style wrap (`{element_name: body}`) and seed scenarios over every
union member of `iso20_exiDocument`.
"""

import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))
sys.path.insert(0, str(REPO_ROOT / "tools"))

import V2Gjson.iso20_common as v2gjson_iso20_common  # noqa: E402

from codegen.fixture_emitter import (  # noqa: E402
    emit_fragment_scenarios,
    harvest_enum_names,
)
from codegen.iso20_common_choices import CHOICES as ISO20_COMMON_CHOICES  # noqa: E402
from codegen.parser import parse_header  # noqa: E402
from conftest import assert_roundtrip  # noqa: E402


ISO20_COMMON_HEADER = (
    REPO_ROOT / "extern" / "libcbv2g" / "include" / "cbv2g" / "iso_20"
    / "iso20_CommonMessages_Datatypes.h"
)

_HEADER_TEXT = ISO20_COMMON_HEADER.read_text()
_SPECS = {s.name: s for s in parse_header(_HEADER_TEXT)}
_ENUMS = harvest_enum_names(_HEADER_TEXT)

# Seed-overrides for fields the per-kind default cannot satisfy. Keys are
# `(struct-type, field-name)`. Populated as length-exact bytes / characters
# fields and constrained scalars surface during the sweep.
ISO20_COMMON_OVERRIDES: dict = {
    # `maxSupportingPointsScheduleTupleType` is xs:unsignedShort with a
    # 1024-biased EXI encoding: libcbv2g subtracts 1024 on encode and adds it
    # on decode, so value 1 round-trips as 1025. Seed at 1024 to land in the
    # schema's valid range.
    ("iso20_ScheduleExchangeReqType", "MaximumSupportingPoints"): 1024,
}


def _discover_document_members():
    """Pull (struct-type, element-name) for each member of `iso20_exiDocument`'s
    inner union, in declaration order. Unlike ISO-2, there is no abstract base
    member to exclude — every union member is a concrete Document root.
    """
    doc_spec = _SPECS["iso20_exiDocument"]
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
            overrides=ISO20_COMMON_OVERRIDES,
            v2gjson=v2gjson_iso20_common,
            namespace_prefix="iso20_",
            choices=ISO20_COMMON_CHOICES,
        )


_SCENARIOS = list(_scenarios())


@pytest.mark.parametrize(
    "scenario", [pytest.param(s, id=s[0]) for s in _SCENARIOS]
)
def test_iso20_common_generated_document_roundtrip(scenario):
    _scenario_id, payload = scenario
    assert_roundtrip("ISO20_COMMON", payload)
