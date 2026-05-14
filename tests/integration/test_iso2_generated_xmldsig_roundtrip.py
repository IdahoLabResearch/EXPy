"""Integration roundtrip over generator-produced ISO2 Xmldsig payloads.

Enumerates the members of the `iso2_xmldsigFragment` union and roundtrips each at
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
        )


_SCENARIOS = list(_scenarios())

# Xmldsig roots that internally contain an XSD-choice the generator cannot honor
# without per-choice-branch support (deferred to #18). Two flavors of choice
# appear here:
#   1. choice across optional members (KeyInfoType, KeyValueType, X509DataType):
#      libcbv2g's encoder emits one branch even when none is marked _isUsed, so
#      both minimal and maximal mis-roundtrip.
#   2. choice_N substructs (PGPDataType) or nested types with choice
#      (TransformType has choice {ANY, XPath}): maximal sets multiple branches
#      and the encoder only keeps one.
_CHOICE_BEARING_SCENARIO_IDS = frozenset({
    # KeyInfoType — choice of {KeyName, KeyValue, RetrievalMethod, X509Data,
    # PGPData, SPKIData, MgmtData, ANY}.
    "KeyInfo__minimal",
    "KeyInfo__maximal",
    # KeyValueType — choice of {DSAKeyValue, RSAKeyValue, ANY}.
    "KeyValue__minimal",
    "KeyValue__maximal",
    # PGPDataType — choice_1 vs choice_2 substructs.
    "PGPData__maximal",
    # TransformType — choice of {ANY, XPath}; only maximal sets both.
    "Transform__maximal",
    # TransformsType wraps Transform; same root cause.
    "Transforms__maximal",
    # ReferenceType.Transforms.Transform inherits the TransformType choice.
    "Reference__maximal",
    # RetrievalMethodType.Transforms.Transform — same.
    "RetrievalMethod__maximal",
    # SignedInfoType embeds Reference[].Transforms.Transform.
    "SignedInfo__maximal",
    # SignatureType embeds SignedInfo and KeyInfo.
    "Signature__maximal",
    # X509DataType — choice of {X509IssuerSerial, X509SKI, X509SubjectName,
    # X509Certificate, X509CRL, ANY}. Encoder emits X509IssuerSerial by default.
    "X509Data__minimal",
    "X509Data__maximal",
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
def test_iso2_generated_xmldsig_roundtrip(scenario):
    _scenario_id, payload = scenario
    assert_roundtrip("ISO2", payload, kind="xmldsig")
