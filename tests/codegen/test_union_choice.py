"""Anonymous union of named `choice_N` substructs (xmldsig PGPData/X509Data pattern)."""

import textwrap

from codegen.emitter import emit_get_json, emit_get_doc
from codegen.parser import parse_header


PGP_HEADER = """\
struct din_PGPDataType {
    union {
        struct {
            struct {
                uint8_t bytes[din_base64Binary_BYTES_SIZE];
                uint16_t bytesLen;
            } PGPKeyID;
            struct {
                uint8_t bytes[din_anyType_BYTES_SIZE];
                uint16_t bytesLen;
            } ANY;
            unsigned int ANY_isUsed:1;
        } choice_1;
        unsigned int choice_1_isUsed:1;

        struct {
            struct {
                uint8_t bytes[din_base64Binary_BYTES_SIZE];
                uint16_t bytesLen;
            } PGPKeyPacket;
        } choice_2;
        unsigned int choice_2_isUsed:1;
    };
};
"""


def test_parser_lifts_union_choice_substructs_into_fields():
    [spec] = parse_header(PGP_HEADER)
    assert [f.name for f in spec.fields] == ["choice_1", "choice_2"]
    c1, c2 = spec.fields
    assert c1.kind == "choice"
    assert c1.optional is True
    assert [s.name for s in c1.subfields] == ["PGPKeyID", "ANY"]
    assert c1.subfields[0].kind == "bytes"
    assert c1.subfields[1].kind == "bytes"
    assert c1.subfields[1].optional is True
    assert c2.kind == "choice"
    assert c2.optional is True
    assert [s.name for s in c2.subfields] == ["PGPKeyPacket"]


def test_emit_get_json_union_choice():
    [spec] = parse_header(PGP_HEADER)
    out = emit_get_json(spec)
    assert "if (PGPDataTypeDoc.choice_1_isUsed) {" in out
    assert 'outJson["choice_1"]["PGPKeyID"]["bytesLen"] = PGPDataTypeDoc.choice_1.PGPKeyID.bytesLen;' in out
    assert "if (PGPDataTypeDoc.choice_1.ANY_isUsed) {" in out
    assert 'outJson["choice_1"]["ANY"]["bytesLen"] = PGPDataTypeDoc.choice_1.ANY.bytesLen;' in out
    assert "if (PGPDataTypeDoc.choice_2_isUsed) {" in out
    assert 'outJson["choice_2"]["PGPKeyPacket"]["bytesLen"] = PGPDataTypeDoc.choice_2.PGPKeyPacket.bytesLen;' in out


def test_emit_get_doc_union_choice():
    [spec] = parse_header(PGP_HEADER)
    out = emit_get_doc(spec)
    assert 'if (PGPDataTypeJson.contains("choice_1")) {' in out
    assert "outDoc.choice_1_isUsed = 1;" in out
    assert 'outDoc.choice_1.PGPKeyID.bytesLen = PGPDataTypeJson["choice_1"]["PGPKeyID"]["bytesLen"].template get<uint16_t>();' in out
    assert 'if (PGPDataTypeJson["choice_1"].contains("ANY")) {' in out
    assert "outDoc.choice_1.ANY_isUsed = 1;" in out
    assert "outDoc.choice_1.ANY_isUsed = 0;" in out
    assert "outDoc.choice_2_isUsed = 0;" in out
