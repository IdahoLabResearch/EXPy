"""Anonymous array of scalar/enum elements (no `struct` keyword on element type)."""

import textwrap

from codegen.emitter import emit_get_json, emit_get_doc
from codegen.parser import TypeSpec, Field, parse_header


def test_parser_detects_scalar_array_kind():
    [spec] = parse_header("""
        struct din_PaymentOptionsType {
            struct {
                din_paymentOptionType array[din_paymentOptionType_2_ARRAY_SIZE];
                uint16_t arrayLen;
            } PaymentOption;
        };
    """)
    [f] = spec.fields
    assert f.name == "PaymentOption"
    assert f.kind == "scalar_array"
    assert f.c_type == "din_paymentOptionType"
    assert f.size_macro == "din_paymentOptionType_2_ARRAY_SIZE"


def test_emit_get_json_for_scalar_array():
    spec = TypeSpec(
        name="din_PaymentOptionsType",
        fields=[
            Field(
                name="PaymentOption",
                kind="scalar_array",
                c_type="din_paymentOptionType",
                size_macro="din_paymentOptionType_2_ARRAY_SIZE",
            )
        ],
    )
    expected = textwrap.dedent(
        """\
        json getJson_PaymentOptionsType(const struct din_PaymentOptionsType& PaymentOptionsTypeDoc) {
            json outJson = json::object();

            outJson["PaymentOption"]["arrayLen"] = PaymentOptionsTypeDoc.PaymentOption.arrayLen;
            for (uint16_t i = 0; i < PaymentOptionsTypeDoc.PaymentOption.arrayLen && i < din_paymentOptionType_2_ARRAY_SIZE; i++) {
                outJson["PaymentOption"]["array"][i] = PaymentOptionsTypeDoc.PaymentOption.array[i];
            }

            return outJson;
        }
        """
    )
    assert emit_get_json(spec) == expected


def test_emit_get_doc_for_scalar_array():
    spec = TypeSpec(
        name="din_PaymentOptionsType",
        fields=[
            Field(
                name="PaymentOption",
                kind="scalar_array",
                c_type="din_paymentOptionType",
                size_macro="din_paymentOptionType_2_ARRAY_SIZE",
            )
        ],
    )
    expected = textwrap.dedent(
        """\
        struct din_PaymentOptionsType getDoc_PaymentOptionsType(const json& PaymentOptionsTypeJson) {
            struct din_PaymentOptionsType outDoc;
            init_din_PaymentOptionsType(&outDoc);

            outDoc.PaymentOption.arrayLen = PaymentOptionsTypeJson["PaymentOption"]["arrayLen"].template get<uint16_t>();
            for (uint16_t i = 0; i < outDoc.PaymentOption.arrayLen && i < din_paymentOptionType_2_ARRAY_SIZE; i++) {
                outDoc.PaymentOption.array[i] = PaymentOptionsTypeJson["PaymentOption"]["array"][i].template get<din_paymentOptionType>();
            }

            return outDoc;
        }
        """
    )
    assert emit_get_doc(spec) == expected
