"""Integration roundtrip over generator-produced ISO2 Xmldsig payloads.

Enumerates the members of the `iso2_xmldsigFragment` union and roundtrips each at
minimal + maximal, with per-XSD-choice-branch coverage driven by the manifest
in `tools/codegen/iso2_choices.py`.
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


def _discover_xmldsig_members():
    """Pull (struct-type, element-name) for each member of `iso2_xmldsigFragment`'s
    inner union, in declaration order.
    """
    spec = _SPECS["iso2_xmldsigFragment"]
    return [(f.c_type, f.name) for f in spec.fields if f.kind == "struct"]


_XMLDSIG_TYPES = _discover_xmldsig_members()


def _scenarios():
    for type_name, element in _XMLDSIG_TYPES:
        yield from emit_fragment_scenarios(
            type_name,
            element_name=element,
            specs=_SPECS,
            enum_names=_ENUMS,
            overrides=None,
            v2gjson=v2gjson_iso2,
            namespace_prefix="iso2_",
            choices=ISO2_CHOICES,
        )


_SCENARIOS = list(_scenarios())

# PGPDataType sequence-2 path remains broken upstream in libcbv2g even after
# ADR-0007: the encoder at grammar 65's else-branch reads from
# choice_1.PGPKeyPacket regardless of which substruct the caller populated,
# and grammar transitions 65→67 collapse seq 2 into a seq-1-without-PGPKeyID
# shape. Fixing this needs grammar rewrites in iso2_msgDefEncoder.c — tracked
# in #22.
_CHOICE_2_BROKEN = frozenset({"PGPData__choice_choice_2"})


def _param(scenario):
    sid = scenario[0]
    if sid in _CHOICE_2_BROKEN:
        return pytest.param(
            scenario,
            id=sid,
            marks=pytest.mark.xfail(
                reason="libcbv2g PGPData seq-2 grammar bug (#22)",
                strict=True,
            ),
        )
    return pytest.param(scenario, id=sid)


@pytest.mark.parametrize("scenario", [_param(s) for s in _SCENARIOS])
def test_iso2_generated_xmldsig_roundtrip(scenario):
    _scenario_id, payload = scenario
    assert_roundtrip("ISO2", payload, kind="xmldsig")
