"""Array of anonymous {bytes, bytesLen} sub-structs (e.g. iso2_SubCertificatesType)."""

import textwrap

from codegen.emitter import emit_get_json, emit_get_doc
from codegen.parser import parse_header


def test_parser_detects_bytes_array_kind():
    [spec] = parse_header("""
        struct iso2_SubCertificatesType {
            struct {
                struct {
                    uint8_t bytes[iso2_certificateType_BYTES_SIZE];
                    uint16_t bytesLen;
                } array[iso2_certificateType_4_ARRAY_SIZE];
                uint16_t arrayLen;
            } Certificate;
        };
    """)
    [f] = spec.fields
    assert f.name == "Certificate"
    assert f.kind == "bytes_array"
    assert f.size_macro == "iso2_certificateType_4_ARRAY_SIZE"
    assert f.elem_size_macro == "iso2_certificateType_BYTES_SIZE"


def test_emit_get_json_bytes_array():
    [spec] = parse_header("""
        struct iso2_SubCertificatesType {
            struct {
                struct {
                    uint8_t bytes[iso2_certificateType_BYTES_SIZE];
                    uint16_t bytesLen;
                } array[iso2_certificateType_4_ARRAY_SIZE];
                uint16_t arrayLen;
            } Certificate;
        };
    """)
    expected = textwrap.dedent(
        """\
        json getJson_SubCertificatesType(const struct iso2_SubCertificatesType& SubCertificatesTypeDoc) {
            json outJson = json::object();

            outJson["Certificate"]["arrayLen"] = SubCertificatesTypeDoc.Certificate.arrayLen;
            for (uint16_t i = 0; i < SubCertificatesTypeDoc.Certificate.arrayLen && i < iso2_certificateType_4_ARRAY_SIZE; i++) {
                outJson["Certificate"]["array"][i]["bytesLen"] = SubCertificatesTypeDoc.Certificate.array[i].bytesLen;
                for (uint16_t j = 0; j < SubCertificatesTypeDoc.Certificate.array[i].bytesLen && j < iso2_certificateType_BYTES_SIZE; j++) {
                    outJson["Certificate"]["array"][i]["bytes"][j] = SubCertificatesTypeDoc.Certificate.array[i].bytes[j];
                }
            }

            return outJson;
        }
        """
    )
    assert emit_get_json(spec) == expected


def test_emit_get_doc_bytes_array():
    [spec] = parse_header("""
        struct iso2_SubCertificatesType {
            struct {
                struct {
                    uint8_t bytes[iso2_certificateType_BYTES_SIZE];
                    uint16_t bytesLen;
                } array[iso2_certificateType_4_ARRAY_SIZE];
                uint16_t arrayLen;
            } Certificate;
        };
    """)
    expected = textwrap.dedent(
        """\
        struct iso2_SubCertificatesType getDoc_SubCertificatesType(const json& SubCertificatesTypeJson) {
            struct iso2_SubCertificatesType outDoc;
            init_iso2_SubCertificatesType(&outDoc);

            outDoc.Certificate.arrayLen = SubCertificatesTypeJson["Certificate"]["arrayLen"].template get<uint16_t>();
            for (uint16_t i = 0; i < outDoc.Certificate.arrayLen && i < iso2_certificateType_4_ARRAY_SIZE; i++) {
                outDoc.Certificate.array[i].bytesLen = SubCertificatesTypeJson["Certificate"]["array"][i]["bytesLen"].template get<uint16_t>();
                for (uint16_t j = 0; j < outDoc.Certificate.array[i].bytesLen && j < iso2_certificateType_BYTES_SIZE; j++) {
                    outDoc.Certificate.array[i].bytes[j] = SubCertificatesTypeJson["Certificate"]["array"][i]["bytes"][j].template get<uint8_t>();
                }
            }

            return outDoc;
        }
        """
    )
    assert emit_get_doc(spec) == expected
