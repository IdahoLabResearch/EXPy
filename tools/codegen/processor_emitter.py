"""Emit `<Namespace>Processor.cpp` and `<Namespace>Processor.hpp` from the
per-Namespace policy plus the set of root types present in its libcbv2g header.

Root-type set is derived from the parsed Datatypes header (which of
`<c_prefix>_exiDocument` / `<c_prefix>_exiFragment` / `<c_prefix>_xmldsigFragment`
appear as TypeSpec names). Everything else — C prefix, marshaler prefix,
V2G_Message wrap policy, include paths — is closed-set per-Namespace policy and
lives in NAMESPACES below.
"""

from __future__ import annotations

import argparse
import sys
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class NamespacePolicy:
    namespace: str
    c_prefix: str
    marshaler_prefix: str
    datatypes_header: str
    encoder_header: str
    decoder_header: str
    marshalers_include: str
    processor_hpp_filename: str
    unwrap_v2g_message: bool


NAMESPACES: dict[str, NamespacePolicy] = {
    "ISO20_ACDP": NamespacePolicy(
        namespace="ISO20_ACDP",
        c_prefix="iso20_acdp",
        marshaler_prefix="acdp_",
        datatypes_header="cbv2g/iso_20/iso20_ACDP_Datatypes.h",
        encoder_header="cbv2g/iso_20/iso20_ACDP_Encoder.h",
        decoder_header="cbv2g/iso_20/iso20_ACDP_Decoder.h",
        # Path relative to the generated cpp's location (src/generated/), so
        # the marshalers sibling include resolves via "" search.
        marshalers_include="ISO20ACDP_marshalers.generated.cpp",
        processor_hpp_filename="ISO20ACDPProcessor.generated.hpp",
        unwrap_v2g_message=False,
    ),
    "ISO2": NamespacePolicy(
        namespace="ISO2",
        c_prefix="iso2",
        marshaler_prefix="",
        datatypes_header="cbv2g/iso_2/iso2_msgDefDatatypes.h",
        encoder_header="cbv2g/iso_2/iso2_msgDefEncoder.h",
        decoder_header="cbv2g/iso_2/iso2_msgDefDecoder.h",
        marshalers_include="ISO2_marshalers.generated.cpp",
        processor_hpp_filename="ISO2Processor.generated.hpp",
        unwrap_v2g_message=True,
    ),
    "DIN": NamespacePolicy(
        namespace="DIN",
        c_prefix="din",
        marshaler_prefix="",
        datatypes_header="cbv2g/din/din_msgDefDatatypes.h",
        encoder_header="cbv2g/din/din_msgDefEncoder.h",
        decoder_header="cbv2g/din/din_msgDefDecoder.h",
        marshalers_include="DIN_marshalers.generated.cpp",
        processor_hpp_filename="DINProcessor.generated.hpp",
        unwrap_v2g_message=True,
    ),
    "SAP": NamespacePolicy(
        namespace="SAP",
        c_prefix="appHand",
        marshaler_prefix="",
        datatypes_header="cbv2g/app_handshake/appHand_Datatypes.h",
        encoder_header="cbv2g/app_handshake/appHand_Encoder.h",
        decoder_header="cbv2g/app_handshake/appHand_Decoder.h",
        marshalers_include="SAP_marshalers.generated.cpp",
        processor_hpp_filename="SupportedAppProtocolProcessor.generated.hpp",
        unwrap_v2g_message=False,
    ),
    "ISO20_AC": NamespacePolicy(
        namespace="ISO20_AC",
        c_prefix="iso20_ac",
        marshaler_prefix="ac_",
        datatypes_header="cbv2g/iso_20/iso20_AC_Datatypes.h",
        encoder_header="cbv2g/iso_20/iso20_AC_Encoder.h",
        decoder_header="cbv2g/iso_20/iso20_AC_Decoder.h",
        marshalers_include="ISO20AC_marshalers.generated.cpp",
        processor_hpp_filename="ISO20ACProcessor.generated.hpp",
        unwrap_v2g_message=False,
    ),
    "ISO20_DC": NamespacePolicy(
        namespace="ISO20_DC",
        c_prefix="iso20_dc",
        marshaler_prefix="dc_",
        datatypes_header="cbv2g/iso_20/iso20_DC_Datatypes.h",
        encoder_header="cbv2g/iso_20/iso20_DC_Encoder.h",
        decoder_header="cbv2g/iso_20/iso20_DC_Decoder.h",
        marshalers_include="ISO20DC_marshalers.generated.cpp",
        processor_hpp_filename="ISO20DCProcessor.generated.hpp",
        unwrap_v2g_message=False,
    ),
    "ISO20_COMMON": NamespacePolicy(
        namespace="ISO20_COMMON",
        c_prefix="iso20",
        marshaler_prefix="",
        datatypes_header="cbv2g/iso_20/iso20_CommonMessages_Datatypes.h",
        encoder_header="cbv2g/iso_20/iso20_CommonMessages_Encoder.h",
        decoder_header="cbv2g/iso_20/iso20_CommonMessages_Decoder.h",
        marshalers_include="ISO20Common_marshalers.generated.cpp",
        processor_hpp_filename="ISO20CommonProcessor.generated.hpp",
        unwrap_v2g_message=False,
    ),
    "ISO20_WPT": NamespacePolicy(
        namespace="ISO20_WPT",
        c_prefix="iso20_wpt",
        marshaler_prefix="wpt_",
        datatypes_header="cbv2g/iso_20/iso20_WPT_Datatypes.h",
        encoder_header="cbv2g/iso_20/iso20_WPT_Encoder.h",
        decoder_header="cbv2g/iso_20/iso20_WPT_Decoder.h",
        marshalers_include="ISO20WPT_marshalers.generated.cpp",
        processor_hpp_filename="ISO20WPTProcessor.generated.hpp",
        unwrap_v2g_message=False,
    ),
}


def detect_root_types(spec_names: list[str], c_prefix: str) -> set[str]:
    """Return the subset of {"exiDocument","exiFragment","xmldsigFragment"} that
    appear as `<c_prefix>_<root>` in the parsed header's TypeSpec names."""
    roots = {"exiDocument", "exiFragment", "xmldsigFragment"}
    return {r for r in roots if f"{c_prefix}_{r}" in spec_names}


def _decode_block(policy: NamespacePolicy, root: str, fn_name: str) -> str:
    cp = policy.c_prefix
    mp = policy.marshaler_prefix
    # V2G_Message unwrap is a Document-only quirk of DIN/ISO2 marshalers.
    # Fragments and xmldsig roots don't wrap their content; pass through.
    unwrap = (
        '["V2G_Message"]'
        if policy.unwrap_v2g_message and root == "exiDocument"
        else ""
    )
    return f"""\
    decoded_data* {fn_name}(const uint8_t* data, size_t size) {{
        try {{
            {cp}_{root} outDoc;
            exi_bitstream_t inEXI;

            exi_bitstream_init(&inEXI, const_cast<uint8_t*>(data), size, 0, nullptr);
            int rc = decode_{cp}_{root}(&inEXI, &outDoc);

            decoded_data* result = new decoded_data{{nullptr, rc}};
            if (rc != 0) {{
                return result;
            }}

            json outJson = getJson_{mp}{root}(outDoc){unwrap};

            static string jsonString;
            jsonString = outJson.dump(4);

            result->json = jsonString.c_str();
            return result;
        }} catch (...) {{
            return new decoded_data{{nullptr, EXPY_ERROR__MARSHALER_INPUT}};
        }}
    }}
"""


def _encode_block(policy: NamespacePolicy, root: str, fn_name: str) -> str:
    cp = policy.c_prefix
    mp = policy.marshaler_prefix
    if policy.unwrap_v2g_message and root == "exiDocument":
        wrap_lines = (
            "            // Caller passes Header/Body at the root; rewrap to match the marshaler.\n"
            "            json wrapped;\n"
            '            wrapped["V2G_Message"] = inJson;\n\n'
        )
        marshaler_arg = "wrapped"
    else:
        wrap_lines = ""
        marshaler_arg = "inJson"
    return f"""\
    encoded_data* {fn_name}(const char* inString) {{
        try {{
            json inJson = json::parse(inString);

{wrap_lines}            {cp}_{root} inDoc;
            inDoc = getDoc_{mp}{root}({marshaler_arg});

            uint8_t* stream = new uint8_t[65536];
            exi_bitstream_t outEXI;
            size_t pos1 = 0;

            exi_bitstream_init(&outEXI, stream, 65536, pos1, nullptr);
            int rc = encode_{cp}_{root}(&outEXI, &inDoc);

            encoded_data* result = new encoded_data;
            result->status = rc;
            result->size = exi_bitstream_get_length(&outEXI);
            result->buffer = new uint8_t[result->size];
            memcpy(result->buffer, stream, result->size);

            delete[] stream;
            return result;
        }} catch (...) {{
            encoded_data* result = new encoded_data;
            result->status = EXPY_ERROR__MARSHALER_INPUT;
            result->size = 0;
            result->buffer = nullptr;
            return result;
        }}
    }}
"""


_FREES = """\
    void free_encoded_data(encoded_data* data) {
        if (data) {
            delete[] data->buffer;
            delete data;
        }
    }

    void free_decoded_data(decoded_data* data) {
        // The JSON payload itself lives in a function-static string; only the
        // wrapper struct is heap-allocated.
        if (data) {
            delete data;
        }
    }
"""


def emit_processor_cpp(policy: NamespacePolicy, root_types: set[str]) -> str:
    parts: list[str] = [
        "// Auto-generated by tools/codegen/processor_emitter.py. DO NOT EDIT.\n",
        f"// Namespace: {policy.namespace}\n",
        "\n",
        "#include <cstddef>\n",
        "#include <cstdint>\n",
        "#include <cstring>\n",
        "#include <string>\n",
        "\n",
        f"#include <{policy.datatypes_header}>\n",
        f"#include <{policy.decoder_header}>\n",
        f"#include <{policy.encoder_header}>\n",
        "\n",
        "#include <common.hpp>\n",
        f'#include "{policy.processor_hpp_filename}"\n',
        "\n",
        "#include <nlohmann/json.hpp>\n",
        "using json = nlohmann::json;\n",
        "\n",
        "using namespace std;\n",
        "\n\n",
        f'#include "{policy.marshalers_include}"\n',
        "\n\n",
        'extern "C" {\n\n',
    ]
    if "exiDocument" in root_types:
        parts.append(_decode_block(policy, "exiDocument", "decodeFromPythonBytes"))
        parts.append("\n")
        parts.append(_encode_block(policy, "exiDocument", "encodeFromPythonDict"))
        parts.append("\n")
    if "exiFragment" in root_types:
        parts.append(
            _decode_block(policy, "exiFragment", "decode_fragment_FromPythonBytes")
        )
        parts.append("\n")
        parts.append(
            _encode_block(policy, "exiFragment", "encode_fragment_FromPythonDict")
        )
        parts.append("\n")
    if "xmldsigFragment" in root_types:
        parts.append(
            _decode_block(
                policy, "xmldsigFragment", "decode_xmldsig_FromPythonBytes"
            )
        )
        parts.append("\n")
        parts.append(
            _encode_block(
                policy, "xmldsigFragment", "encode_xmldsig_FromPythonDict"
            )
        )
        parts.append("\n")
    parts.append(_FREES)
    parts.append("}\n")
    return "".join(parts)


def _hpp_decls(root_types: set[str]) -> list[str]:
    decls: list[str] = []
    if "exiDocument" in root_types:
        decls.append(
            "decoded_data* decodeFromPythonBytes(const uint8_t* data, size_t size);"
        )
        decls.append("encoded_data* encodeFromPythonDict(const char* inString);")
    if "exiFragment" in root_types:
        decls.append(
            "decoded_data* decode_fragment_FromPythonBytes(const uint8_t* data, size_t size);"
        )
        decls.append(
            "encoded_data* encode_fragment_FromPythonDict(const char* inString);"
        )
    if "xmldsigFragment" in root_types:
        decls.append(
            "decoded_data* decode_xmldsig_FromPythonBytes(const uint8_t* data, size_t size);"
        )
        decls.append(
            "encoded_data* encode_xmldsig_FromPythonDict(const char* inString);"
        )
    return decls


def emit_processor_hpp(policy: NamespacePolicy, root_types: set[str]) -> str:
    decls = _hpp_decls(root_types)
    body_lines = [
        "// Auto-generated by tools/codegen/processor_emitter.py. DO NOT EDIT.",
        f"// Namespace: {policy.namespace}",
        "",
        "#pragma once",
        "",
        "#include <cstddef>",
        "#include <cstdint>",
        "",
        'extern "C" {',
        "",
        "struct decoded_data {",
        "    const char* json;",
        "    int status;",
        "};",
        "",
        "struct encoded_data {",
        "    uint8_t* buffer;",
        "    size_t size;",
        "    int status;",
        "};",
        "",
        *decls,
        "",
        "void free_encoded_data(encoded_data* data);",
        "void free_decoded_data(decoded_data* data);",
        "",
        "}",
        "",
    ]
    return "\n".join(body_lines)


def main(argv: list[str] | None = None) -> int:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--namespace", required=True, choices=sorted(NAMESPACES.keys()))
    p.add_argument("--header", required=True, type=Path,
                   help="libcbv2g *_Datatypes.h to parse for root-type detection")
    p.add_argument("--out-cpp", required=True, type=Path)
    p.add_argument("--out-hpp", required=True, type=Path)
    args = p.parse_args(argv)

    from .parser import parse_header

    policy = NAMESPACES[args.namespace]
    specs = parse_header(args.header.read_text())
    root_types = detect_root_types([s.name for s in specs], policy.c_prefix)
    if not root_types:
        print(
            f"error: no root types ({policy.c_prefix}_exiDocument etc.) found in "
            f"{args.header}",
            file=sys.stderr,
        )
        return 1

    args.out_cpp.parent.mkdir(parents=True, exist_ok=True)
    args.out_hpp.parent.mkdir(parents=True, exist_ok=True)
    args.out_cpp.write_text(emit_processor_cpp(policy, root_types))
    args.out_hpp.write_text(emit_processor_hpp(policy, root_types))
    return 0


if __name__ == "__main__":
    sys.exit(main())
