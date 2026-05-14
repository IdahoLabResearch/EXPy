"""Array of anonymous {characters, charactersLen} sub-structs."""

import textwrap

from codegen.emitter import emit_get_json, emit_get_doc
from codegen.parser import parse_header


def test_parser_detects_chars_array_kind():
    [spec] = parse_header("""
        struct din_ListOfRootCertificateIDsType {
            struct {
                struct {
                    char characters[din_RootCertificateID_CHARACTER_SIZE];
                    uint16_t charactersLen;
                } array[din_rootCertificateIDType_5_ARRAY_SIZE];
                uint16_t arrayLen;
            } RootCertificateID;
        };
    """)
    [f] = spec.fields
    assert f.name == "RootCertificateID"
    assert f.kind == "chars_array"
    assert f.size_macro == "din_rootCertificateIDType_5_ARRAY_SIZE"
    assert f.elem_size_macro == "din_RootCertificateID_CHARACTER_SIZE"


def test_emit_get_json_chars_array():
    [spec] = parse_header("""
        struct din_ListOfRootCertificateIDsType {
            struct {
                struct {
                    char characters[din_RootCertificateID_CHARACTER_SIZE];
                    uint16_t charactersLen;
                } array[din_rootCertificateIDType_5_ARRAY_SIZE];
                uint16_t arrayLen;
            } RootCertificateID;
        };
    """)
    expected = textwrap.dedent(
        """\
        json getJson_ListOfRootCertificateIDsType(const struct din_ListOfRootCertificateIDsType& ListOfRootCertificateIDsTypeDoc) {
            json outJson = json::object();

            outJson["RootCertificateID"]["arrayLen"] = ListOfRootCertificateIDsTypeDoc.RootCertificateID.arrayLen;
            for (uint16_t i = 0; i < ListOfRootCertificateIDsTypeDoc.RootCertificateID.arrayLen && i < din_rootCertificateIDType_5_ARRAY_SIZE; i++) {
                outJson["RootCertificateID"]["array"][i]["charactersLen"] = ListOfRootCertificateIDsTypeDoc.RootCertificateID.array[i].charactersLen;
                for (uint16_t j = 0; j < ListOfRootCertificateIDsTypeDoc.RootCertificateID.array[i].charactersLen && j < din_RootCertificateID_CHARACTER_SIZE; j++) {
                    outJson["RootCertificateID"]["array"][i]["characters"][j] = ListOfRootCertificateIDsTypeDoc.RootCertificateID.array[i].characters[j];
                }
            }

            return outJson;
        }
        """
    )
    assert emit_get_json(spec) == expected


def test_emit_get_doc_chars_array():
    [spec] = parse_header("""
        struct din_ListOfRootCertificateIDsType {
            struct {
                struct {
                    char characters[din_RootCertificateID_CHARACTER_SIZE];
                    uint16_t charactersLen;
                } array[din_rootCertificateIDType_5_ARRAY_SIZE];
                uint16_t arrayLen;
            } RootCertificateID;
        };
    """)
    expected = textwrap.dedent(
        """\
        struct din_ListOfRootCertificateIDsType getDoc_ListOfRootCertificateIDsType(const json& ListOfRootCertificateIDsTypeJson) {
            struct din_ListOfRootCertificateIDsType outDoc;
            init_din_ListOfRootCertificateIDsType(&outDoc);

            outDoc.RootCertificateID.arrayLen = ListOfRootCertificateIDsTypeJson["RootCertificateID"]["arrayLen"].template get<uint16_t>();
            for (uint16_t i = 0; i < outDoc.RootCertificateID.arrayLen && i < din_rootCertificateIDType_5_ARRAY_SIZE; i++) {
                outDoc.RootCertificateID.array[i].charactersLen = ListOfRootCertificateIDsTypeJson["RootCertificateID"]["array"][i]["charactersLen"].template get<uint16_t>();
                for (uint16_t j = 0; j < outDoc.RootCertificateID.array[i].charactersLen && j < din_RootCertificateID_CHARACTER_SIZE; j++) {
                    outDoc.RootCertificateID.array[i].characters[j] = ListOfRootCertificateIDsTypeJson["RootCertificateID"]["array"][i]["characters"][j].template get<char>();
                }
            }

            return outDoc;
        }
        """
    )
    assert emit_get_doc(spec) == expected
