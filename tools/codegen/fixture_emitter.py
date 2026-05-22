"""Schema-driven fixture generator.

Walks `TypeSpec` output from `parser.py` and emits JSON payloads matching the
shape libcbv2g's Processor expects, per the matrix in ADR-0005:
- "minimal": only required fields
- "maximal": all optionals present
- per-XSD-choice branch: one variant per choice union (driven by a
  hand-curated `choices` manifest — see `iso2_choices.py` / `din_choices.py`)

XSD-choice handling: when a root's type tree contains a struct listed in the
`choices` manifest, the generator replaces the root's min/max pair with one
scenario per branch. For nested choices reached during emission, the
generator defaults to branch 0 — those nested types get their own per-branch
coverage at whichever top-level root reaches them directly.
"""

from __future__ import annotations

import itertools
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
# Choice manifest: c_struct_name -> list of choice *groups* (one group per
# independent xs:choice inside that struct). Each group is a list of
# *branches*, and each branch is a list of field names that get `_isUsed=1`
# together.
ChoiceManifest = dict[str, list[list[list[str]]]]
# Per-scenario selection: c_struct_name -> list of active branches (one per
# choice group in manifest order). Each active branch is a list of field
# names.
ChoiceSelections = dict[str, list[list[str]]]


def emit_document_scenarios(
    type_name: str,
    *,
    element_name: str,
    specs: dict[str, TypeSpec],
    enum_names: set[str],
    overrides: SeedOverrides | None = None,
    v2gjson: Any = None,
    namespace_prefix: str = "",
    choices: ChoiceManifest | None = None,
):
    """Yield ``(scenario_id, payload)`` for `type_name` wrapped as a Document.

    Payload shape: ``{"Body": {element_name: <body>}}``.
    Scenario ids: ``{element_name}__minimal`` / ``{element_name}__maximal``,
    or ``{element_name}__choice_{branch_field}`` when a choice point is
    reachable in `type_name`'s type tree via `choices`.
    """
    yield from _emit_scenarios(
        type_name,
        element_name=element_name,
        specs=specs,
        enum_names=enum_names,
        overrides=overrides,
        v2gjson=v2gjson,
        namespace_prefix=namespace_prefix,
        choices=choices,
        wrap=lambda body: {"Body": {element_name: body}},
    )


def emit_fragment_scenarios(
    type_name: str,
    *,
    element_name: str,
    specs: dict[str, TypeSpec],
    enum_names: set[str],
    overrides: SeedOverrides | None = None,
    v2gjson: Any = None,
    namespace_prefix: str = "",
    choices: ChoiceManifest | None = None,
):
    """Yield ``(scenario_id, payload)`` for `type_name` wrapped as a Fragment.

    Payload shape: ``{element_name: <body>}`` (no ``Body`` wrapper). The
    Fragment encode/decode pair expects this single-element shape.
    """
    yield from _emit_scenarios(
        type_name,
        element_name=element_name,
        specs=specs,
        enum_names=enum_names,
        overrides=overrides,
        v2gjson=v2gjson,
        namespace_prefix=namespace_prefix,
        choices=choices,
        wrap=lambda body: {element_name: body},
    )


def _emit_scenarios(
    type_name: str,
    *,
    element_name: str,
    specs: dict[str, TypeSpec],
    enum_names: set[str],
    overrides: SeedOverrides | None,
    v2gjson: Any,
    namespace_prefix: str,
    choices: ChoiceManifest | None,
    wrap,
):
    if type_name not in specs:
        raise GeneratorError(f"type {type_name!r} not present in specs")
    spec = specs[type_name]
    choice_point = _find_topmost_choice_point(spec, specs=specs, choices=choices or {})
    if choice_point is not None:
        cp_struct, groups = choice_point
        for combination in itertools.product(*groups):
            active_per_group = list(combination)
            selections: ChoiceSelections = {cp_struct: active_per_group}
            body = emit_body(
                spec,
                variant="maximal",
                specs=specs,
                enum_names=enum_names,
                overrides=overrides,
                v2gjson=v2gjson,
                namespace_prefix=namespace_prefix,
                choices=choices,
                choice_selections=selections,
            )
            sid = f"{element_name}__choice_" + "_".join(
                name for branch in active_per_group for name in branch
            )
            yield sid, wrap(body)
        return
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
        yield f"{element_name}__{variant}", wrap(body)


def _find_topmost_choice_point(
    spec: TypeSpec,
    *,
    specs: dict[str, TypeSpec],
    choices: ChoiceManifest,
    visited: set[str] | None = None,
) -> tuple[str, list[list[list[str]]]] | None:
    """Return the first choice-bearing struct reachable from `spec` via DFS,
    or None. Stops descending once it hits a choice struct — nested choices
    inside that branch are handled by the per-branch default in `emit_body`.

    Returns ``(c_struct_name, groups)`` where ``groups`` is the manifest
    entry: a list of independent choice groups, each a list of branches.
    """
    if not choices:
        return None
    if visited is None:
        visited = set()
    if spec.name in visited:
        return None
    visited.add(spec.name)
    if spec.name in choices:
        return spec.name, choices[spec.name]
    for field in spec.fields:
        if field.kind not in ("struct", "array"):
            continue
        nested = specs.get(field.c_type)
        if nested is None:
            continue
        found = _find_topmost_choice_point(
            nested, specs=specs, choices=choices, visited=visited
        )
        if found is not None:
            return found
    return None


def emit_body(
    spec: TypeSpec,
    *,
    variant: str,
    specs: dict[str, TypeSpec],
    enum_names: set[str],
    overrides: SeedOverrides | None = None,
    v2gjson: Any = None,
    namespace_prefix: str = "",
    choices: ChoiceManifest | None = None,
    choice_selections: ChoiceSelections | None = None,
) -> dict[str, Any]:
    """Emit a payload body (no Document/Fragment wrapping) for `spec`.

    If `spec.name` is in `choices`, only the fields belonging to the active
    branch of each choice group (from `choice_selections`, defaulting to
    branch 0 of every group) are emitted — non-choice members of the same
    struct are still emitted normally.
    """
    overrides = overrides or {}
    skip_choice_members: set[str] = set()
    if choices and spec.name in choices:
        groups = choices[spec.name]
        active_per_group = (choice_selections or {}).get(spec.name)
        if active_per_group is None:
            active_per_group = [group[0] for group in groups]
        all_choice_members = {
            name for group in groups for branch in group for name in branch
        }
        active_members = {name for branch in active_per_group for name in branch}
        skip_choice_members = all_choice_members - active_members
    body: dict[str, Any] = {}
    for field in spec.fields:
        if field.name in skip_choice_members:
            continue
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
            choices=choices,
            choice_selections=choice_selections,
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
    choices: ChoiceManifest | None = None,
    choice_selections: ChoiceSelections | None = None,
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
            choices=choices,
            choice_selections=choice_selections,
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
            choices=choices,
            choice_selections=choice_selections,
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
    if field.kind == "choice":
        # XSD-choice substruct (e.g. PGPDataType.choice_1): recurse into the
        # parser-captured `subfields` under this name. The roundtrip is only
        # correct when exactly one choice branch is set, which the per-branch
        # work in #18 covers; min/max here are expected to xfail.
        out: dict[str, Any] = {}
        for sub in field.subfields or ():
            if sub.optional and variant == "minimal":
                continue
            out[sub.name] = _seed_field(
                spec_name=f"{spec_name}.{field.name}",
                field=sub,
                variant=variant,
                specs=specs,
                enum_names=enum_names,
                overrides=overrides,
                v2gjson=v2gjson,
                namespace_prefix=namespace_prefix,
                choices=choices,
                choice_selections=choice_selections,
            )
        return out
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
            "v2gjson=<expy.v2gjson.{namespace}> to emit_body()"
        )
    short = c_type
    if namespace_prefix and short.startswith(namespace_prefix):
        short = short[len(namespace_prefix):]
    cls = getattr(v2gjson, short, None)
    if cls is None:
        return 0
    return next(iter(cls)).value
