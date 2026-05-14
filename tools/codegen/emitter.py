"""Emit C++ Marshaler bodies for a TypeSpec."""

from __future__ import annotations

from .parser import TypeSpec, Field


INDENT = "    "


def _marshaler_suffix(c_type: str) -> str:
    """Suffix used on `getJson_<X>` / `getDoc_<X>` for a struct field's type.

    Namespace-prefixed types (`din_CostType`, `iso2_FooType`, ...) drop the prefix
    so the generated marshaler is named after the bare type. Library-provided
    typedef'd structs (`exi_signed_t`) keep their full name — those marshalers
    are hand-written in `common.cpp` under their full name.
    """
    if "_" in c_type and c_type.split("_", 1)[0] in {"din", "iso2", "iso20", "appHand"}:
        return c_type.split("_", 1)[1]
    return c_type


def _short(struct_name: str) -> str:
    """`din_CostType` -> `CostType`. Used for the marshaler's own name."""
    return struct_name.split("_", 1)[1]


# ----- getJson_ -----------------------------------------------------------------


def emit_get_json(spec: TypeSpec) -> str:
    short = _short(spec.name)
    doc = f"{short}Doc"
    out: list[str] = [
        f"json getJson_{short}(const struct {spec.name}& {doc}) {{",
        # Initialize as object so empty/all-optional structs serialize as `{}`,
        # not the default-null json, which wouldn't round-trip via getDoc_.
        f"{INDENT}json outJson = json::object();",
        "",
    ]
    for f in spec.fields:
        out.extend(_field_get_json(f, "outJson", doc, indent_lvl=1))
    out.extend(["", f"{INDENT}return outJson;", "}"])
    return "\n".join(out) + "\n"


def _field_get_json(f: Field, json_prefix: str, doc_prefix: str, indent_lvl: int) -> list[str]:
    """Emit this field, including any `if (..._isUsed)` wrapping."""
    ind = INDENT * indent_lvl
    if f.optional:
        out = [f"{ind}if ({doc_prefix}.{f.name}_isUsed) {{"]
        out.extend(_field_get_json_body(f, json_prefix, doc_prefix, indent_lvl + 1))
        out.append(f"{ind}}}")
        return out
    return _field_get_json_body(f, json_prefix, doc_prefix, indent_lvl)


def _field_get_json_body(f: Field, json_prefix: str, doc_prefix: str, indent_lvl: int) -> list[str]:
    ind = INDENT * indent_lvl
    name = f.name
    fj = f'{json_prefix}["{name}"]'
    fd = f"{doc_prefix}.{name}"

    if f.kind == "scalar":
        return [f"{ind}{fj} = {fd};"]
    if f.kind == "struct":
        return [f"{ind}{fj} = getJson_{_marshaler_suffix(f.c_type)}({fd});"]
    if f.kind == "characters":
        return [
            f'{ind}{fj}["charactersLen"] = {fd}.charactersLen;',
            f"{ind}for (uint16_t i = 0; i < {fd}.charactersLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fj}["characters"][i] = {fd}.characters[i];',
            f"{ind}}}",
        ]
    if f.kind == "bytes":
        return [
            f'{ind}{fj}["bytesLen"] = {fd}.bytesLen;',
            f"{ind}for (uint16_t i = 0; i < {fd}.bytesLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fj}["bytes"][i] = {fd}.bytes[i];',
            f"{ind}}}",
        ]
    if f.kind == "array":
        elem_short = _marshaler_suffix(f.c_type)
        return [
            f'{ind}{fj}["arrayLen"] = {fd}.arrayLen;',
            f"{ind}for (uint16_t i = 0; i < {fd}.arrayLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fj}["array"][i] = getJson_{elem_short}({fd}.array[i]);',
            f"{ind}}}",
        ]
    if f.kind == "scalar_array":
        return [
            f'{ind}{fj}["arrayLen"] = {fd}.arrayLen;',
            f"{ind}for (uint16_t i = 0; i < {fd}.arrayLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fj}["array"][i] = {fd}.array[i];',
            f"{ind}}}",
        ]
    if f.kind == "chars_array":
        return [
            f'{ind}{fj}["arrayLen"] = {fd}.arrayLen;',
            f"{ind}for (uint16_t i = 0; i < {fd}.arrayLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fj}["array"][i]["charactersLen"] = {fd}.array[i].charactersLen;',
            f"{ind}{INDENT}for (uint16_t j = 0; j < {fd}.array[i].charactersLen && j < {f.elem_size_macro}; j++) {{",
            f'{ind}{INDENT}{INDENT}{fj}["array"][i]["characters"][j] = {fd}.array[i].characters[j];',
            f"{ind}{INDENT}}}",
            f"{ind}}}",
        ]
    if f.kind == "choice":
        out: list[str] = []
        for sub in f.subfields:
            out.extend(_field_get_json(sub, fj, fd, indent_lvl))
        return out
    raise ValueError(f"unknown field kind: {f.kind}")


# ----- getDoc_ ------------------------------------------------------------------


def emit_get_doc(spec: TypeSpec) -> str:
    short = _short(spec.name)
    j = f"{short}Json"
    out: list[str] = [
        f"struct {spec.name} getDoc_{short}(const json& {j}) {{",
        f"{INDENT}struct {spec.name} outDoc;",
        f"{INDENT}init_{spec.name}(&outDoc);",
        "",
    ]
    for f in spec.fields:
        out.extend(_field_get_doc(f, j, "outDoc", indent_lvl=1))
    out.extend(["", f"{INDENT}return outDoc;", "}"])
    return "\n".join(out) + "\n"


def _field_get_doc(f: Field, json_prefix: str, doc_prefix: str, indent_lvl: int) -> list[str]:
    ind = INDENT * indent_lvl
    if f.optional:
        out = [f'{ind}if ({json_prefix}.contains("{f.name}")) {{']
        out.append(f"{ind}{INDENT}{doc_prefix}.{f.name}_isUsed = 1;")
        out.extend(_field_get_doc_body(f, json_prefix, doc_prefix, indent_lvl + 1))
        out.append(f"{ind}}} else {{")
        out.append(f"{ind}{INDENT}{doc_prefix}.{f.name}_isUsed = 0;")
        out.append(f"{ind}}}")
        return out
    return _field_get_doc_body(f, json_prefix, doc_prefix, indent_lvl)


def _field_get_doc_body(f: Field, json_prefix: str, doc_prefix: str, indent_lvl: int) -> list[str]:
    ind = INDENT * indent_lvl
    name = f.name
    fj = f'{json_prefix}["{name}"]'
    fd = f"{doc_prefix}.{name}"

    if f.kind == "scalar":
        return [f"{ind}{fd} = {fj}.template get<{f.c_type}>();"]
    if f.kind == "struct":
        return [f"{ind}{fd} = getDoc_{_marshaler_suffix(f.c_type)}({fj});"]
    if f.kind == "characters":
        return [
            f'{ind}{fd}.charactersLen = {fj}["charactersLen"].template get<uint16_t>();',
            f"{ind}for (uint16_t i = 0; i < {fd}.charactersLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fd}.characters[i] = {fj}["characters"][i].template get<char>();',
            f"{ind}}}",
        ]
    if f.kind == "bytes":
        return [
            f'{ind}{fd}.bytesLen = {fj}["bytesLen"].template get<uint16_t>();',
            f"{ind}for (uint16_t i = 0; i < {fd}.bytesLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fd}.bytes[i] = {fj}["bytes"][i].template get<uint8_t>();',
            f"{ind}}}",
        ]
    if f.kind == "array":
        elem_short = _marshaler_suffix(f.c_type)
        return [
            f'{ind}{fd}.arrayLen = {fj}["arrayLen"].template get<uint16_t>();',
            f"{ind}for (uint16_t i = 0; i < {fd}.arrayLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fd}.array[i] = getDoc_{elem_short}({fj}["array"][i]);',
            f"{ind}}}",
        ]
    if f.kind == "scalar_array":
        return [
            f'{ind}{fd}.arrayLen = {fj}["arrayLen"].template get<uint16_t>();',
            f"{ind}for (uint16_t i = 0; i < {fd}.arrayLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fd}.array[i] = {fj}["array"][i].template get<{f.c_type}>();',
            f"{ind}}}",
        ]
    if f.kind == "chars_array":
        return [
            f'{ind}{fd}.arrayLen = {fj}["arrayLen"].template get<uint16_t>();',
            f"{ind}for (uint16_t i = 0; i < {fd}.arrayLen && i < {f.size_macro}; i++) {{",
            f'{ind}{INDENT}{fd}.array[i].charactersLen = {fj}["array"][i]["charactersLen"].template get<uint16_t>();',
            f"{ind}{INDENT}for (uint16_t j = 0; j < {fd}.array[i].charactersLen && j < {f.elem_size_macro}; j++) {{",
            f'{ind}{INDENT}{INDENT}{fd}.array[i].characters[j] = {fj}["array"][i]["characters"][j].template get<char>();',
            f"{ind}{INDENT}}}",
            f"{ind}}}",
        ]
    if f.kind == "choice":
        out: list[str] = []
        for sub in f.subfields:
            out.extend(_field_get_doc(sub, fj, fd, indent_lvl))
        return out
    raise ValueError(f"unknown field kind: {f.kind}")
