"""Unit tests for the V2Gjson enum-table emitter."""

from codegen.v2gjson_emitter import emit


def _exec(source: str) -> dict[str, object]:
    ns: dict[str, object] = {}
    exec(source, ns)
    return ns


def test_emits_one_enum_class_per_typedef():
    header = """
    typedef enum {
        ns_widgetKind_RED = 0,
        ns_widgetKind_GREEN = 1
    } ns_widgetKind;
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    cls = ns["widgetKind"]
    assert cls.RED.value == 0  # type: ignore[attr-defined]
    assert cls.GREEN.value == 1  # type: ignore[attr-defined]


def test_class_name_strips_namespace_prefix():
    """Matches the hand-written V2Gjson/iso2.py convention so that
    fixture_emitter._resolve_enum_value finds classes by `c_type[len(prefix):]`.
    """
    header = """
    typedef enum {
        iso20_responseCodeType_OK = 0
    } iso20_responseCodeType;
    """
    src = emit(header, namespace_prefix="iso20_", module_doc="x")
    assert "class responseCodeType(Enum):" in src
    assert "class iso20_responseCodeType" not in src


def test_member_names_are_stripped_of_full_typedef_prefix():
    header = """
    typedef enum {
        ns_ColorType_Red = 0,
        ns_ColorType_Blue = 1
    } ns_ColorType;
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    cls = ns["ColorType"]
    assert {m.name for m in cls} == {"Red", "Blue"}  # type: ignore[attr-defined]


def test_None_member_is_renamed_to_None_underscore():
    """Python keyword guard — matches the hand-written iso2.py pattern."""
    header = """
    typedef enum {
        ns_T_None = 0,
        ns_T_Active = 1
    } ns_T;
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    cls = ns["T"]
    assert cls.None_.value == 0  # type: ignore[attr-defined]


def test_internal_dispatch_enum_is_skipped_not_raised():
    """libcbv2g's `*_generatedFunctionNumbersType` enum has bare-name members
    (no type-name prefix). It is an internal dispatch table — no marshaler
    field is typed as it — so the emitter skips it with a comment marker
    instead of raising or emitting a broken class.
    """
    header = """
    typedef enum {
        ns_AuthorizationReq = 0,
        ns_AuthorizationRes = 1
    } ns_generatedFunctionNumbersType;
    typedef enum {
        ns_otherType_A = 0,
        ns_otherType_B = 1
    } ns_otherType;
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    assert "class generatedFunctionNumbersType" not in src
    assert "skipped ns_generatedFunctionNumbersType" in src
    ns = _exec(src)
    assert ns["otherType"].A.value == 0  # type: ignore[attr-defined]


def test_first_enum_value_resolves_correctly():
    """Drives _resolve_enum_value in fixture_emitter — first iter() value."""
    header = """
    typedef enum {
        ns_responseCodeType_OK = 0,
        ns_responseCodeType_FAIL = 99
    } ns_responseCodeType;
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    cls = ns["responseCodeType"]
    first = next(iter(cls))  # type: ignore[call-overload]
    assert first.value == 0
