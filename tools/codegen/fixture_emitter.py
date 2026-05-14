"""Schema-driven fixture generator.

Walks `TypeSpec` output from `parser.py` and emits JSON payloads matching the
shape libcbv2g's Processor expects, per the matrix in ADR-0005:
- "minimal": only required fields
- "maximal": all optionals present
- per-XSD-choice branch: one variant per choice union
"""

from __future__ import annotations

import re
from typing import Any

from .parser import Field, TypeSpec


_ENUM_TYPEDEF_RE = re.compile(
    r"typedef\s+enum\s*\{[^}]*\}\s*(?P<name>\w+)\s*;", re.DOTALL
)


def harvest_enum_names(header_text: str) -> set[str]:
    """Return the set of `typedef enum { ... } NAME;` names declared in `header_text`."""
    return {m["name"] for m in _ENUM_TYPEDEF_RE.finditer(header_text)}


class GeneratorError(Exception):
    """Raised when the generator cannot produce a valid payload."""


_NUMERIC_C_TYPES = {
    "uint8_t", "uint16_t", "uint32_t", "uint64_t",
    "int8_t", "int16_t", "int32_t", "int64_t",
    # Plain `int X;` in libcbv2g is the C representation of XSD `xs:boolean`.
    "int",
}


def _is_numeric(c_type: str) -> bool:
    return c_type in _NUMERIC_C_TYPES


SeedOverrides = dict[tuple[str, str], Any]


def emit_document_scenarios(
    type_name: str,
    *,
    element_name: str,
    specs: dict[str, TypeSpec],
    enum_names: set[str],
    overrides: SeedOverrides | None = None,
    v2gjson: Any = None,
    namespace_prefix: str = "",
):
    """Yield ``(scenario_id, payload)`` for `type_name` wrapped as a Document.

    Payload shape: ``{"Body": {element_name: <body>}}``.
    Scenario ids: ``{element_name}__minimal`` / ``{element_name}__maximal``.
    """
    if type_name not in specs:
        raise GeneratorError(f"type {type_name!r} not present in specs")
    spec = specs[type_name]
    for variant in ("minimal", "maximal"):
        body = emit_body(
            spec,
            variant=variant,
            specs=specs,
            enum_names=enum_names,
            overrides=overrides,
            v2gjson=v2gjson,
            namespace_prefix=namespace_prefix,
        )
        yield f"{element_name}__{variant}", {"Body": {element_name: body}}


def emit_body(
    spec: TypeSpec,
    *,
    variant: str,
    specs: dict[str, TypeSpec],
    enum_names: set[str],
    overrides: SeedOverrides | None = None,
    v2gjson: Any = None,
    namespace_prefix: str = "",
) -> dict[str, Any]:
    """Emit a payload body (no Document/Fragment wrapping) for `spec`."""
    overrides = overrides or {}
    body: dict[str, Any] = {}
    for field in spec.fields:
        if field.optional and variant == "minimal":
            continue
        body[field.name] = _seed_field(
            spec_name=spec.name,
            field=field,
            variant=variant,
            specs=specs,
            enum_names=enum_names,
            overrides=overrides,
            v2gjson=v2gjson,
            namespace_prefix=namespace_prefix,
        )
    return body


def _seed_field(
    *,
    spec_name: str,
    field: Field,
    variant: str,
    specs: dict[str, TypeSpec],
    enum_names: set[str],
    overrides: SeedOverrides,
    v2gjson: Any,
    namespace_prefix: str,
) -> Any:
    key = (spec_name, field.name)
    if key in overrides:
        return overrides[key]
    if field.kind == "bytes":
        return {"bytes": [0], "bytesLen": 1}
    if field.kind == "characters":
        return {"characters": [120], "charactersLen": 1}
    if field.kind == "scalar":
        if field.c_type in enum_names:
            return _resolve_enum_value(
                field.c_type, v2gjson=v2gjson, namespace_prefix=namespace_prefix
            )
        if _is_numeric(field.c_type):
            return 1
    if field.kind == "struct":
        if field.c_type == "exi_signed_t":
            # libcbv2g's EXI codec canonicalizes leading zeros, so `octets=[0]`
            # encodes as value 0 and decodes back as empty (`octets_count=0`).
            # Use 1 to survive the roundtrip.
            return {
                "data": {"octets": [1], "octets_count": 1},
                "is_negative": 0,
            }
        nested = specs.get(field.c_type)
        if nested is None:
            raise GeneratorError(
                f"struct field {spec_name}.{field.name} references unknown type {field.c_type!r}"
            )
        return emit_body(
            nested,
            variant=variant,
            specs=specs,
            enum_names=enum_names,
            overrides=overrides,
            v2gjson=v2gjson,
            namespace_prefix=namespace_prefix,
        )
    if field.kind == "array":
        nested = specs.get(field.c_type)
        if nested is None:
            raise GeneratorError(
                f"array field {spec_name}.{field.name} references unknown type {field.c_type!r}"
            )
        element = emit_body(
            nested,
            variant=variant,
            specs=specs,
            enum_names=enum_names,
            overrides=overrides,
            v2gjson=v2gjson,
            namespace_prefix=namespace_prefix,
        )
        return {"array": [element], "arrayLen": 1}
    if field.kind == "scalar_array":
        return {"array": [1], "arrayLen": 1}
    if field.kind == "chars_array":
        return {
            "array": [{"characters": [120], "charactersLen": 1}],
            "arrayLen": 1,
        }
    if field.kind == "bytes_array":
        return {
            "array": [{"bytes": [0], "bytesLen": 1}],
            "arrayLen": 1,
        }
    raise GeneratorError(
        f"no seed for {spec_name}.{field.name} (kind={field.kind})"
    )


def _resolve_enum_value(c_type: str, *, v2gjson: Any, namespace_prefix: str) -> int:
    """Look up the first enum value for `c_type` via V2Gjson.

    Falls back to 0 if the matching `Enum` class is missing — per ADR-0005,
    "fall back to 0 if the class is missing." A missing module (caller did not
    pass `v2gjson`) is a programmer error, not a fallback.
    """
    if v2gjson is None:
        raise GeneratorError(
            f"enum-typed scalar of type {c_type} needs a v2gjson module — pass "
            "v2gjson=<V2Gjson.{namespace}> to emit_body()"
        )
    short = c_type
    if namespace_prefix and short.startswith(namespace_prefix):
        short = short[len(namespace_prefix):]
    cls = getattr(v2gjson, short, None)
    if cls is None:
        return 0
    return next(iter(cls)).value
