"""Schema-driven fixture generator: emits min/max payloads from parsed TypeSpecs."""

import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

import expy.v2gjson.iso2 as v2gjson_iso2

from codegen.parser import parse_header
import pytest

from codegen.fixture_emitter import (
    GeneratorError,
    emit_body,
    emit_document_scenarios,
    emit_fragment_scenarios,
    harvest_enum_names,
)


def test_emits_minimal_body_with_seed_override_for_length_constrained_bytes():
    header = """
    struct iso2_SessionSetupReqType {
        struct {
            uint8_t bytes[iso2_evccIDType_BYTES_SIZE];
            uint16_t bytesLen;
        } EVCCID;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec,
        variant="minimal",
        specs={spec.name: spec},
        enum_names=set(),
        overrides={("iso2_SessionSetupReqType", "EVCCID"): {"bytes": [0] * 6, "bytesLen": 6}},
    )

    assert body == {"EVCCID": {"bytes": [0] * 6, "bytesLen": 6}}


def test_emits_minimal_body_with_per_kind_default_for_bytes():
    header = """
    struct iso2_CertificateInstallationReqType {
        struct {
            uint8_t bytes[iso2_certificateType_BYTES_SIZE];
            uint16_t bytesLen;
        } OEMProvisioningCert;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec,
        variant="minimal",
        specs={spec.name: spec},
        enum_names=set(),
    )

    assert body == {"OEMProvisioningCert": {"bytes": [0], "bytesLen": 1}}


def test_emits_minimal_body_with_per_kind_default_for_characters():
    header = """
    struct iso2_SomeType {
        struct {
            char characters[iso2_EVSEID_CHARACTER_SIZE];
            uint16_t charactersLen;
        } EVSEID;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec,
        variant="minimal",
        specs={spec.name: spec},
        enum_names=set(),
    )

    assert body == {"EVSEID": {"characters": [120], "charactersLen": 1}}


def test_emits_minimal_body_with_per_kind_default_for_numeric_scalars():
    header = """
    struct iso2_SomeType {
        uint32_t Count;
        int64_t Timestamp;
        uint8_t Flag;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec,
        variant="minimal",
        specs={spec.name: spec},
        enum_names=set(),
    )

    assert body == {"Count": 1, "Timestamp": 1, "Flag": 1}


def test_minimal_omits_optionals_maximal_includes_them():
    header = """
    struct iso2_SomeType {
        uint32_t Required;
        uint32_t Optional;
        unsigned int Optional_isUsed:1;
    };
    """
    [spec] = parse_header(header)

    minimal = emit_body(
        spec, variant="minimal", specs={spec.name: spec}, enum_names=set()
    )
    maximal = emit_body(
        spec, variant="maximal", specs={spec.name: spec}, enum_names=set()
    )

    assert minimal == {"Required": 1}
    assert maximal == {"Required": 1, "Optional": 1}


def test_emits_enum_scalar_value_from_v2gjson_module():
    header = """
    struct iso2_SessionSetupResType {
        iso2_responseCodeType ResponseCode;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec,
        variant="minimal",
        specs={spec.name: spec},
        enum_names={"iso2_responseCodeType"},
        v2gjson=v2gjson_iso2,
        namespace_prefix="iso2_",
    )

    assert body == {"ResponseCode": v2gjson_iso2.responseCodeType.OK.value}


def test_harvests_enum_typedef_names_from_header_text():
    header = """
    typedef enum {
        iso2_costKindType_relativePricePercentage = 0,
        iso2_costKindType_RenewableGenerationPercentage = 1
    } iso2_costKindType;

    typedef enum {
        iso2_responseCodeType_OK = 0,
        iso2_responseCodeType_FAILED = 4
    } iso2_responseCodeType;
    """

    assert harvest_enum_names(header) == {"iso2_costKindType", "iso2_responseCodeType"}


def test_emit_document_scenarios_wraps_in_body_and_yields_min_and_max():
    header = """
    struct iso2_SessionSetupReqType {
        struct {
            uint8_t bytes[iso2_evccIDType_BYTES_SIZE];
            uint16_t bytesLen;
        } EVCCID;
    };
    """
    specs = {s.name: s for s in parse_header(header)}

    scenarios = list(emit_document_scenarios(
        "iso2_SessionSetupReqType",
        element_name="SessionSetupReq",
        specs=specs,
        enum_names=set(),
        overrides={("iso2_SessionSetupReqType", "EVCCID"): {"bytes": [0] * 6, "bytesLen": 6}},
    ))

    expected_body = {"Body": {"SessionSetupReq": {"EVCCID": {"bytes": [0] * 6, "bytesLen": 6}}}}
    assert [sid for sid, _ in scenarios] == ["SessionSetupReq__minimal", "SessionSetupReq__maximal"]
    assert [p for _, p in scenarios] == [expected_body, expected_body]


def test_enum_lookup_falls_back_to_zero_when_v2gjson_class_missing():
    """Per ADR-0005: drift between header enums and V2Gjson should not crash
    the generator; encode failure surfaces the mismatch instead.
    """
    header = """
    struct iso2_FooType {
        iso2_neverDefinedEnumType Field;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec,
        variant="minimal",
        specs={spec.name: spec},
        enum_names={"iso2_neverDefinedEnumType"},
        v2gjson=v2gjson_iso2,
        namespace_prefix="iso2_",
    )

    assert body == {"Field": 0}


def test_actionable_error_when_v2gjson_module_not_supplied_for_enum_scalar():
    header = """
    struct iso2_FooType {
        iso2_responseCodeType ResponseCode;
    };
    """
    [spec] = parse_header(header)

    with pytest.raises(GeneratorError, match="v2gjson"):
        emit_body(
            spec,
            variant="minimal",
            specs={spec.name: spec},
            enum_names={"iso2_responseCodeType"},
        )


def test_actionable_error_when_requested_type_missing_from_specs():
    with pytest.raises(GeneratorError, match="iso2_NoSuchType"):
        list(emit_document_scenarios(
            "iso2_NoSuchType",
            element_name="NoSuchType",
            specs={},
            enum_names=set(),
        ))


def test_exi_signed_t_struct_uses_builtin_shape():
    """`exi_signed_t` is a libcbv2g builtin (from exi_basetypes.h), not a
    namespace struct. Its JSON shape is fixed and the generator hard-codes it
    rather than trying to recurse into a TypeSpec that doesn't exist.
    """
    header = """
    struct iso2_X509IssuerSerialType {
        exi_signed_t X509SerialNumber;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec, variant="minimal", specs={spec.name: spec}, enum_names=set()
    )

    assert body == {
        "X509SerialNumber": {
            "data": {"octets": [1], "octets_count": 1},
            "is_negative": 0,
        }
    }


def test_struct_kind_recurses_into_nested_typespec():
    header = """
    struct iso2_OuterType {
        struct iso2_InnerType Inner;
    };
    struct iso2_InnerType {
        uint32_t Count;
    };
    """
    specs = {s.name: s for s in parse_header(header)}

    body = emit_body(
        specs["iso2_OuterType"],
        variant="minimal",
        specs=specs,
        enum_names=set(),
    )

    assert body == {"Inner": {"Count": 1}}


def test_empty_struct_recurses_to_empty_dict():
    """libcbv2g emits a placeholder `int _unused` for empty complexTypes (e.g.,
    iso2_SessionStopReqType); the parser strips it, so the spec has zero fields.
    """
    header = """
    struct iso2_EmptyType {
        int _unused;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec, variant="minimal", specs={spec.name: spec}, enum_names=set()
    )

    assert body == {}


def test_array_kind_emits_one_element_list_and_arrayLen():
    header = """
    struct iso2_OuterType {
        struct {
            struct iso2_ProfileEntryType array[iso2_ProfileEntryType_24_ARRAY_SIZE];
            uint16_t arrayLen;
        } ProfileEntry;
    };
    struct iso2_ProfileEntryType {
        uint32_t Start;
    };
    """
    specs = {s.name: s for s in parse_header(header)}

    body = emit_body(
        specs["iso2_OuterType"],
        variant="minimal",
        specs=specs,
        enum_names=set(),
    )

    assert body == {"ProfileEntry": {"array": [{"Start": 1}], "arrayLen": 1}}


def test_scalar_array_kind_emits_one_element_list_and_arrayLen():
    header = """
    struct iso2_OuterType {
        struct {
            uint8_t array[iso2_someListType_4_ARRAY_SIZE];
            uint16_t arrayLen;
        } Values;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec, variant="minimal", specs={spec.name: spec}, enum_names=set()
    )

    assert body == {"Values": {"array": [1], "arrayLen": 1}}


def test_chars_array_kind_emits_one_characters_element():
    header = """
    struct iso2_OuterType {
        struct {
            struct {
                char characters[iso2_FieldName_CHARACTER_SIZE];
                uint16_t charactersLen;
            } array[iso2_FieldName_3_ARRAY_SIZE];
            uint16_t arrayLen;
        } Names;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec, variant="minimal", specs={spec.name: spec}, enum_names=set()
    )

    assert body == {
        "Names": {
            "array": [{"characters": [120], "charactersLen": 1}],
            "arrayLen": 1,
        }
    }


def test_emit_fragment_scenarios_wraps_in_element_only_no_body():
    """Fragment payloads are `{element_name: body}` — no `Body` wrapper, unlike
    Document scenarios. The encoder/decoder Fragment pair expects this shape.
    """
    header = """
    struct iso2_AuthorizationReqType {
        struct {
            uint8_t bytes[iso2_genChallengeType_BYTES_SIZE];
            uint16_t bytesLen;
        } GenChallenge;
    };
    """
    specs = {s.name: s for s in parse_header(header)}

    scenarios = list(emit_fragment_scenarios(
        "iso2_AuthorizationReqType",
        element_name="AuthorizationReq",
        specs=specs,
        enum_names=set(),
        overrides={("iso2_AuthorizationReqType", "GenChallenge"): {"bytes": [0] * 16, "bytesLen": 16}},
    ))

    expected = {"AuthorizationReq": {"GenChallenge": {"bytes": [0] * 16, "bytesLen": 16}}}
    assert [sid for sid, _ in scenarios] == ["AuthorizationReq__minimal", "AuthorizationReq__maximal"]
    assert [p for _, p in scenarios] == [expected, expected]


def test_choice_kind_emits_subfields_under_choice_name():
    """`kind="choice"` (e.g. PGPDataType.choice_1) is an XSD-choice substruct.
    V2Gjson consumes it as `{choice_N: {<subfields>}}` in the JSON payload, so
    the generator recurses into the subfields under that name. The minimal
    variant omits optional subfields; the maximal includes them.
    """
    header = """
    struct iso2_PGPDataType {
        union {
            struct {
                struct {
                    uint8_t bytes[iso2_base64Binary_BYTES_SIZE];
                    uint16_t bytesLen;
                } PGPKeyID;
                struct {
                    uint8_t bytes[iso2_base64Binary_BYTES_SIZE];
                    uint16_t bytesLen;
                } PGPKeyPacket;
                unsigned int PGPKeyPacket_isUsed:1;
            } choice_1;
            unsigned int choice_1_isUsed:1;
        };
    };
    """
    specs = {s.name: s for s in parse_header(header)}

    minimal = emit_body(
        specs["iso2_PGPDataType"], variant="maximal", specs=specs, enum_names=set()
    )

    assert minimal == {
        "choice_1": {
            "PGPKeyID": {"bytes": [0], "bytesLen": 1},
            "PGPKeyPacket": {"bytes": [0], "bytesLen": 1},
        }
    }


def test_bytes_array_kind_emits_one_bytes_element():
    header = """
    struct iso2_OuterType {
        struct {
            struct {
                uint8_t bytes[iso2_certType_BYTES_SIZE];
                uint16_t bytesLen;
            } array[iso2_certType_4_ARRAY_SIZE];
            uint16_t arrayLen;
        } SubCerts;
    };
    """
    [spec] = parse_header(header)

    body = emit_body(
        spec, variant="minimal", specs={spec.name: spec}, enum_names=set()
    )

    assert body == {
        "SubCerts": {
            "array": [{"bytes": [0], "bytesLen": 1}],
            "arrayLen": 1,
        }
    }
