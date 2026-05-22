"""Parse libcbv2g `*_Datatypes.h` headers into TypeSpecs the emitter consumes."""

from __future__ import annotations

import re
from dataclasses import dataclass, field as dc_field
from typing import List, Optional


# Field kinds:
#   "scalar"       — plain scalar (uint32_t, int8_t, char, enum types, ...)
#   "characters"   — anon { char characters[SIZE]; uint16_t charactersLen; }
#   "bytes"        — anon { uint8_t bytes[SIZE]; uint16_t bytesLen; }
#   "raw_bytes"    — bare `uint8_t NAME[SIZE];` (no companion lenLen envelope).
#                    Used by libcbv2g's `exi_unsigned_t.octets` typedef-struct.
#   "struct"       — named nested `struct din_OtherType NAME;`
#   "array"        — anon { struct din_T array[SIZE]; uint16_t arrayLen; }
#   "scalar_array" — anon { <scalar> array[SIZE]; uint16_t arrayLen; }
#   "chars_array"  — anon { struct { char characters[N]; uint16_t charactersLen; } array[]; uint16_t arrayLen; }
#   "bytes_array"  — anon { struct { uint8_t bytes[N];     uint16_t bytesLen;     } array[]; uint16_t arrayLen; }
#   "choice"       — an XSD-choice sub-struct inside an anonymous union; carries subfields.
@dataclass
class Field:
    name: str
    kind: str = "scalar"
    c_type: str = ""
    size_macro: Optional[str] = None
    elem_size_macro: Optional[str] = None
    optional: bool = False
    subfields: List["Field"] = dc_field(default_factory=list)


@dataclass
class TypeSpec:
    name: str
    fields: List[Field] = dc_field(default_factory=list)
    # True when the struct is declared via `typedef struct { ... } name;`
    # or `typedef struct name { ... } name;`. The typedef name is the
    # canonical C type — and, for V2Gjson, the canonical Python symbol —
    # so the emitter should NOT strip the namespace prefix from it.
    typedef: bool = False
    # True when the struct body contains a statement the parser cannot
    # model (e.g. a pointer field `uint8_t* x;`). The emitter must skip
    # such specs rather than synthesise a half-broken constructor.
    has_unparseable: bool = False


_STRUCT_HEADER_RE = re.compile(r"struct\s+(?P<name>\w+)\s*\{", re.MULTILINE)
_ANON_TYPEDEF_STRUCT_HEADER_RE = re.compile(
    r"typedef\s+struct\s*\{", re.MULTILINE
)
_IS_USED_RE = re.compile(r"^\s*unsigned\s+int\s+(?P<name>\w+)_isUsed:1\s*;\s*$")
_SCALAR_RE = re.compile(
    r"^\s*(?P<type>(?:unsigned\s+|signed\s+)?[A-Za-z_]\w*)\s+(?P<name>\w+)\s*(?::\s*\d+\s*)?;\s*$"
)
_NESTED_STRUCT_RE = re.compile(r"^\s*struct\s+(?P<type>\w+)\s+(?P<name>\w+)\s*;\s*$")
_RAW_BYTES_RE = re.compile(
    r"^\s*uint8_t\s+(?P<name>\w+)\[(?P<size>\w+)\]\s*;\s*$"
)
_CHARACTERS_INNER_RE = re.compile(
    r"^\s*char\s+characters\[(?P<size>\w+)\]\s*;\s*uint16_t\s+charactersLen\s*;\s*$",
    re.DOTALL,
)
_BYTES_INNER_RE = re.compile(
    r"^\s*uint8_t\s+bytes\[(?P<size>\w+)\]\s*;\s*uint16_t\s+bytesLen\s*;\s*$",
    re.DOTALL,
)
_ARRAY_INNER_RE = re.compile(
    r"^\s*struct\s+(?P<type>\w+)\s+array\[(?P<size>\w+)\]\s*;\s*uint16_t\s+arrayLen\s*;\s*$",
    re.DOTALL,
)
_SCALAR_ARRAY_INNER_RE = re.compile(
    r"^\s*(?P<type>(?:unsigned\s+|signed\s+)?[A-Za-z_]\w*)\s+array\[(?P<size>\w+)\]\s*;\s*uint16_t\s+arrayLen\s*;\s*$",
    re.DOTALL,
)
_CHARS_ARRAY_INNER_RE = re.compile(
    r"^\s*struct\s*\{\s*char\s+characters\[(?P<elem_size>\w+)\]\s*;\s*uint16_t\s+charactersLen\s*;\s*\}\s+array\[(?P<size>\w+)\]\s*;\s*uint16_t\s+arrayLen\s*;\s*$",
    re.DOTALL,
)
_BYTES_ARRAY_INNER_RE = re.compile(
    r"^\s*struct\s*\{\s*uint8_t\s+bytes\[(?P<elem_size>\w+)\]\s*;\s*uint16_t\s+bytesLen\s*;\s*\}\s+array\[(?P<size>\w+)\]\s*;\s*uint16_t\s+arrayLen\s*;\s*$",
    re.DOTALL,
)


def parse_header(text: str) -> List[TypeSpec]:
    text = _strip_comments(text)
    specs: List[TypeSpec] = []
    for header_match in _STRUCT_HEADER_RE.finditer(text):
        body, _end = _extract_balanced_body(text, header_match.end() - 1)
        # Detect `typedef struct NAME { ... }` so the emitter can keep the
        # canonical name verbatim.
        prefix = text[: header_match.start()].rstrip()
        is_typedef = prefix.endswith("typedef")
        fields, has_unparseable = _parse_fields_with_status(body)
        specs.append(
            TypeSpec(
                name=header_match["name"],
                fields=fields,
                typedef=is_typedef,
                has_unparseable=has_unparseable,
            )
        )
    for header_match in _ANON_TYPEDEF_STRUCT_HEADER_RE.finditer(text):
        open_brace = text.index("{", header_match.start())
        body, close_idx = _extract_balanced_body(text, open_brace)
        after = text[close_idx + 1 :]
        name_match = re.match(r"\s*(?P<name>\w+)\s*;", after)
        if not name_match:
            continue
        fields, has_unparseable = _parse_fields_with_status(body)
        specs.append(
            TypeSpec(
                name=name_match["name"],
                fields=fields,
                typedef=True,
                has_unparseable=has_unparseable,
            )
        )
    return specs


def _strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    text = re.sub(r"//[^\n]*", "", text)
    return text


def _extract_balanced_body(text: str, open_brace_idx: int) -> tuple[str, int]:
    assert text[open_brace_idx] == "{"
    depth = 0
    i = open_brace_idx
    while i < len(text):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                return text[open_brace_idx + 1 : i], i
        i += 1
    raise ValueError("unbalanced braces")


def _parse_fields(body: str) -> List[Field]:
    fields, _ = _parse_fields_with_status(body)
    return fields


_POINTER_FIELD_RE = re.compile(r"^\s*[A-Za-z_]\w*\s*\*\s*\w+\s*;\s*$")


def _parse_fields_with_status(body: str) -> tuple[List[Field], bool]:
    """Returns (fields, has_unparseable). The flag is True if any statement
    in the body could not be classified into a Field — e.g. a pointer field
    we cannot model on the JSON side.
    """
    fields: List[Field] = []
    optional_names: set[str] = set()
    has_unparseable = False
    pos = 0

    while pos < len(body):
        m = re.match(r"\s+", body[pos:])
        if m:
            pos += m.end()
            continue
        if pos >= len(body):
            break

        rest = body[pos:]

        # Anonymous union. Two patterns:
        #   - `union { struct din_T1 N1; struct din_T2 N2; ... };` (BodyType)
        #     where each member is a named nested struct optionally guarded by a
        #     sibling `_isUsed` bitfield AFTER the union. Treat as transparent.
        #   - `union { struct {...} choice_1; ... }; (PGPDataType / X509Data)
        #     where each choice_N is an anon substruct of multiple fields.
        # Both cases reduce to: recursively parse the union body the same way as
        # a struct body, and lift the resulting fields into the parent list.
        if re.match(r"union\s*\{", rest):
            open_idx = rest.index("{")
            union_body, close_rel = _extract_balanced_body(rest, open_idx)
            after = rest[close_rel + 1 :]
            close_match = re.match(r"\s*;", after)
            if close_match is None:
                raise ValueError("expected `;` after anon union")
            fields.extend(_parse_fields(union_body))
            pos += close_rel + 1 + close_match.end()
            continue

        # Anonymous nested `struct { ... } NAME;`
        if re.match(r"struct\s*\{", rest):
            open_idx = rest.index("{")
            inner, close_rel = _extract_balanced_body(rest, open_idx)
            after = rest[close_rel + 1 :]
            name_match = re.match(r"\s*(?P<name>\w+)\s*;", after)
            if not name_match:
                raise ValueError("expected name after anon struct")
            fields.append(_classify_anon_struct(name_match["name"], inner))
            pos += close_rel + 1 + name_match.end()
            continue

        semi = body.find(";", pos)
        if semi == -1:
            break
        stmt = body[pos : semi + 1]
        pos = semi + 1

        is_used = _IS_USED_RE.match(stmt)
        if is_used:
            optional_names.add(is_used["name"])
            continue

        nested = _NESTED_STRUCT_RE.match(stmt)
        if nested:
            fields.append(
                Field(name=nested["name"], kind="struct", c_type=nested["type"])
            )
            continue

        raw_bytes = _RAW_BYTES_RE.match(stmt)
        if raw_bytes:
            fields.append(
                Field(
                    name=raw_bytes["name"],
                    kind="raw_bytes",
                    size_macro=raw_bytes["size"],
                )
            )
            continue

        if _POINTER_FIELD_RE.match(stmt):
            has_unparseable = True
            continue

        scalar = _SCALAR_RE.match(stmt)
        if scalar:
            # `int _unused;` is libcbv2g's placeholder for semantically empty
            # complexTypes (e.g., DIN's SessionStopType, IntervalType). It carries
            # no information; skipping it lets these structs round-trip as `{}`.
            if scalar["name"] == "_unused":
                continue
            c_type = scalar["type"]
            # `exi_signed_t` is a typedef'd struct from libcbv2g's exi_basetypes.
            # It uses the same struct-style marshalers as named nested structs.
            if c_type == "exi_signed_t":
                fields.append(
                    Field(name=scalar["name"], kind="struct", c_type=c_type)
                )
            else:
                fields.append(
                    Field(name=scalar["name"], kind="scalar", c_type=c_type)
                )

    for f in fields:
        if f.name in optional_names:
            f.optional = True
    return fields, has_unparseable


def _legacy_unused(union_body: str) -> List[Field]:
    """A union body like `struct {...} choice_1; unsigned int choice_1_isUsed:1; ...`.

    The inner `_isUsed` markers live alongside their substructs inside the
    union. We recursively parse each choice substruct as a fresh field list.
    """
    fields: List[Field] = []
    optional_names: set[str] = set()
    pos = 0

    while pos < len(union_body):
        m = re.match(r"\s+", union_body[pos:])
        if m:
            pos += m.end()
            continue
        if pos >= len(union_body):
            break

        rest = union_body[pos:]
        if re.match(r"struct\s*\{", rest):
            open_idx = rest.index("{")
            inner, close_rel = _extract_balanced_body(rest, open_idx)
            after = rest[close_rel + 1 :]
            name_match = re.match(r"\s*(?P<name>\w+)\s*;", after)
            if not name_match:
                raise ValueError("expected name after choice substruct")
            fields.append(
                Field(
                    name=name_match["name"],
                    kind="choice",
                    subfields=_parse_fields(inner),
                )
            )
            pos += close_rel + 1 + name_match.end()
            continue

        semi = union_body.find(";", pos)
        if semi == -1:
            break
        stmt = union_body[pos : semi + 1]
        pos = semi + 1
        is_used = _IS_USED_RE.match(stmt)
        if is_used:
            optional_names.add(is_used["name"])

    for f in fields:
        if f.name in optional_names:
            f.optional = True
    return fields


def _classify_anon_struct(field_name: str, inner: str) -> Field:
    # Check chars_array / bytes_array BEFORE characters / bytes — array forms
    # embed the scalar pattern.
    chars_array_m = _CHARS_ARRAY_INNER_RE.match(inner)
    if chars_array_m:
        return Field(
            name=field_name,
            kind="chars_array",
            size_macro=chars_array_m["size"],
            elem_size_macro=chars_array_m["elem_size"],
        )
    bytes_array_m = _BYTES_ARRAY_INNER_RE.match(inner)
    if bytes_array_m:
        return Field(
            name=field_name,
            kind="bytes_array",
            size_macro=bytes_array_m["size"],
            elem_size_macro=bytes_array_m["elem_size"],
        )
    chars = _CHARACTERS_INNER_RE.match(inner)
    if chars:
        return Field(
            name=field_name, kind="characters", size_macro=chars["size"]
        )
    bytes_m = _BYTES_INNER_RE.match(inner)
    if bytes_m:
        return Field(name=field_name, kind="bytes", size_macro=bytes_m["size"])
    array_m = _ARRAY_INNER_RE.match(inner)
    if array_m:
        return Field(
            name=field_name,
            kind="array",
            c_type=array_m["type"],
            size_macro=array_m["size"],
        )
    scalar_array_m = _SCALAR_ARRAY_INNER_RE.match(inner)
    if scalar_array_m:
        return Field(
            name=field_name,
            kind="scalar_array",
            c_type=scalar_array_m["type"],
            size_macro=scalar_array_m["size"],
        )
    # Fall through: treat as an XSD-choice substruct whose body is itself a
    # nested field list (e.g. PGPDataType.choice_1).
    return Field(name=field_name, kind="choice", subfields=_parse_fields(inner))
