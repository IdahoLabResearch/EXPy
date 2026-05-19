"""Unit tests for the Processor cpp/hpp emitter."""

from codegen.processor_emitter import (
    NamespacePolicy,
    emit_processor_cpp,
    emit_processor_hpp,
    NAMESPACES,
)


def _acdp_like_policy() -> NamespacePolicy:
    """A doc-only policy mirroring ISO20_ACDP — smallest tracer-bullet shape."""
    return NamespacePolicy(
        namespace="ISO20_ACDP",
        c_prefix="iso20_acdp",
        marshaler_prefix="acdp_",
        datatypes_header="cbv2g/iso_20/iso20_ACDP_Datatypes.h",
        encoder_header="cbv2g/iso_20/iso20_ACDP_Encoder.h",
        decoder_header="cbv2g/iso_20/iso20_ACDP_Decoder.h",
        marshalers_include="ISO20ACDP_marshalers.generated.cpp",
        processor_hpp_filename="ISO20ACDPProcessor.generated.hpp",
        unwrap_v2g_message=False,
    )


def test_emit_cpp_for_doc_only_namespace_calls_libcbv2g_document_symbols():
    """Tracer bullet: doc-only Processor.cpp wires decode/encode to the
    namespace's `<c_prefix>_exiDocument` libcbv2g symbols and to the
    `getJson_<marshaler_prefix>exiDocument` / `getDoc_<marshaler_prefix>exiDocument`
    marshalers."""
    policy = _acdp_like_policy()
    root_types = {"exiDocument"}  # derived from header specs; doc-only here

    cpp = emit_processor_cpp(policy, root_types)

    # Decode path: declares the C struct, calls libcbv2g decoder, calls marshaler.
    assert "iso20_acdp_exiDocument outDoc" in cpp
    assert "decode_iso20_acdp_exiDocument(&inEXI, &outDoc)" in cpp
    assert "getJson_acdp_exiDocument(outDoc)" in cpp

    # Encode path: declares the C struct, calls marshaler, calls libcbv2g encoder.
    assert "iso20_acdp_exiDocument inDoc" in cpp
    assert "getDoc_acdp_exiDocument(inJson)" in cpp
    assert "encode_iso20_acdp_exiDocument(&outEXI, &inDoc)" in cpp

    # Frees are unconditional regardless of which roots are present.
    assert "free_encoded_data" in cpp
    assert "free_decoded_data" in cpp

    # Doc-only: must NOT emit fragment / xmldsig wrappers.
    assert "decode_fragment_FromPythonBytes" not in cpp
    assert "decode_xmldsig_FromPythonBytes" not in cpp


def test_emit_hpp_for_doc_only_namespace_declares_document_surface_only():
    """The hpp publishes the same extern "C" surface that the cpp defines.
    Doc-only namespaces declare only the four document/free symbols.
    """
    policy = _acdp_like_policy()
    root_types = {"exiDocument"}

    hpp = emit_processor_hpp(policy, root_types)

    assert "#pragma once" in hpp
    assert 'extern "C" {' in hpp
    # Shared decoded_data/encoded_data layout — every Processor exposes them.
    assert "struct decoded_data" in hpp
    assert "struct encoded_data" in hpp
    # Document methods declared.
    assert "decoded_data* decodeFromPythonBytes(" in hpp
    assert "encoded_data* encodeFromPythonDict(" in hpp
    # Frees declared.
    assert "void free_encoded_data(encoded_data* data);" in hpp
    assert "void free_decoded_data(decoded_data* data);" in hpp
    # Doc-only: no fragment / xmldsig method declarations.
    assert "decode_fragment_FromPythonBytes" not in hpp
    assert "decode_xmldsig_FromPythonBytes" not in hpp


def _common_like_policy() -> NamespacePolicy:
    """A doc+frag+xmldsig policy mirroring ISO20_COMMON (empty marshaler_prefix)."""
    return NamespacePolicy(
        namespace="ISO20_COMMON",
        c_prefix="iso20",
        marshaler_prefix="",
        datatypes_header="cbv2g/iso_20/iso20_CommonMessages_Datatypes.h",
        encoder_header="cbv2g/iso_20/iso20_CommonMessages_Encoder.h",
        decoder_header="cbv2g/iso_20/iso20_CommonMessages_Decoder.h",
        marshalers_include="ISO20Common_marshalers.generated.cpp",
        processor_hpp_filename="ISO20CommonProcessor.generated.hpp",
        unwrap_v2g_message=False,
    )


def test_emit_cpp_with_fragment_emits_fragment_wrappers_using_libcbv2g_symbols():
    """When the header has exiFragment, the Processor.cpp emits
    decode_fragment_FromPythonBytes and encode_fragment_FromPythonDict bound to
    `<c_prefix>_exiFragment` libcbv2g symbols.
    """
    policy = _common_like_policy()
    cpp = emit_processor_cpp(policy, {"exiDocument", "exiFragment"})

    assert "decode_fragment_FromPythonBytes" in cpp
    assert "encode_fragment_FromPythonDict" in cpp
    assert "iso20_exiFragment outDoc" in cpp
    assert "decode_iso20_exiFragment(&inEXI, &outDoc)" in cpp
    assert "encode_iso20_exiFragment(&outEXI, &inDoc)" in cpp
    # Empty marshaler_prefix → bare `getJson_exiFragment`.
    assert "getJson_exiFragment(outDoc)" in cpp
    assert "getDoc_exiFragment(inJson)" in cpp
    # Still no xmldsig (only frag in this test).
    assert "xmldsigFragment" not in cpp


def test_emit_cpp_with_xmldsig_emits_xmldsig_wrappers_using_libcbv2g_symbols():
    """When the header has xmldsigFragment, the Processor.cpp emits
    decode_xmldsig_FromPythonBytes and encode_xmldsig_FromPythonDict bound to
    `<c_prefix>_xmldsigFragment` libcbv2g symbols.
    """
    policy = _common_like_policy()
    cpp = emit_processor_cpp(policy, {"exiDocument", "xmldsigFragment"})

    assert "decode_xmldsig_FromPythonBytes" in cpp
    assert "encode_xmldsig_FromPythonDict" in cpp
    assert "decode_iso20_xmldsigFragment(&inEXI, &outDoc)" in cpp
    assert "encode_iso20_xmldsigFragment(&outEXI, &inDoc)" in cpp
    assert "getJson_xmldsigFragment(outDoc)" in cpp
    assert "getDoc_xmldsigFragment(inJson)" in cpp


def test_emit_hpp_declares_fragment_and_xmldsig_methods_when_present():
    policy = _common_like_policy()
    hpp = emit_processor_hpp(
        policy, {"exiDocument", "exiFragment", "xmldsigFragment"}
    )
    assert "decoded_data* decode_fragment_FromPythonBytes(" in hpp
    assert "encoded_data* encode_fragment_FromPythonDict(" in hpp
    assert "decoded_data* decode_xmldsig_FromPythonBytes(" in hpp
    assert "encoded_data* encode_xmldsig_FromPythonDict(" in hpp


def test_emit_cpp_with_v2g_message_unwrap_elides_and_rewraps_wrapper_key():
    """DIN and ISO2 marshalers expose a `V2G_Message` wrapper at the root.
    The EVerest JSON shape elides it: decode strips it, encode wraps callers'
    JSON in `{"V2G_Message": ...}` before invoking the marshaler. Only the
    Document path is wrapped — fragments and xmldsig stay as-is.
    """
    policy = NamespacePolicy(
        namespace="DIN",
        c_prefix="din",
        marshaler_prefix="",
        datatypes_header="cbv2g/din/din_msgDefDatatypes.h",
        encoder_header="cbv2g/din/din_msgDefEncoder.h",
        decoder_header="cbv2g/din/din_msgDefDecoder.h",
        marshalers_include="DIN_marshalers.generated.cpp",
        processor_hpp_filename="DINProcessor.generated.hpp",
        unwrap_v2g_message=True,
    )
    cpp = emit_processor_cpp(policy, {"exiDocument"})

    # Decode: marshaler returns wrapped JSON, CLI surface unwraps via ["V2G_Message"]
    assert 'getJson_exiDocument(outDoc)["V2G_Message"]' in cpp
    # Encode: caller's JSON gets wrapped in {"V2G_Message": ...} before marshaler
    assert 'wrapped["V2G_Message"] = inJson;' in cpp
    assert "getDoc_exiDocument(wrapped)" in cpp


def test_emit_cpp_without_v2g_message_unwrap_keeps_marshaler_json_as_is():
    """ACDP / ISO20 namespaces don't have the V2G_Message wrapper. Their
    Document path passes the marshaler output / input through untouched.
    """
    policy = _acdp_like_policy()
    cpp = emit_processor_cpp(policy, {"exiDocument"})

    assert 'getJson_acdp_exiDocument(outDoc)["V2G_Message"]' not in cpp
    assert "V2G_Message" not in cpp


def test_detect_root_types_returns_only_roots_present_in_specs():
    """The header is the authority for which root types the Processor exposes.
    `detect_root_types` looks at parsed TypeSpec names and returns the subset
    of {exiDocument, exiFragment, xmldsigFragment} that appear with the policy's
    c_prefix.
    """
    from codegen.processor_emitter import detect_root_types

    # ACDP header: only exiDocument
    acdp_names = ["iso20_acdp_FooType", "iso20_acdp_exiDocument"]
    assert detect_root_types(acdp_names, "iso20_acdp") == {"exiDocument"}

    # COMMON header: all three
    common_names = [
        "iso20_FooType",
        "iso20_exiDocument",
        "iso20_exiFragment",
        "iso20_xmldsigFragment",
    ]
    assert detect_root_types(common_names, "iso20") == {
        "exiDocument",
        "exiFragment",
        "xmldsigFragment",
    }

    # Disambiguation: c_prefix=iso20 must not match iso20_acdp_exiDocument.
    mixed = ["iso20_acdp_exiDocument", "iso20_acdp_FooType"]
    assert detect_root_types(mixed, "iso20") == set()
