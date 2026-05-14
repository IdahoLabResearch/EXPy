"""Fields typed `exi_signed_t` need helper-function marshaling (it's a typedef'd struct)."""

from codegen.emitter import emit_get_json, emit_get_doc
from codegen.parser import parse_header


def test_exi_signed_t_uses_helper_marshalers():
    [spec] = parse_header("""
        struct din_X509IssuerSerialType {
            exi_signed_t X509SerialNumber;
        };
    """)
    out_json = emit_get_json(spec)
    out_doc = emit_get_doc(spec)
    assert 'outJson["X509SerialNumber"] = getJson_exi_signed_t(X509IssuerSerialTypeDoc.X509SerialNumber);' in out_json
    assert 'outDoc.X509SerialNumber = getDoc_exi_signed_t(X509IssuerSerialTypeJson["X509SerialNumber"]);' in out_doc
