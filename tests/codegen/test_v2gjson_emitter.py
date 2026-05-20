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


def test_emits_constructor_with_required_positional_scalar():
    """A struct with one required scalar field emits a constructor whose
    only parameter is that field (positional), and the returned dict carries
    the value verbatim under the field's name.
    """
    header = """
    typedef enum {
        ns_unused_PLACEHOLDER = 0
    } ns_unused;
    struct ns_Widget {
        int16_t Count;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["Widget"]
    assert ctor(7) == {"Count": 7}  # type: ignore[operator]


def test_optional_field_is_keyword_only_and_omitted_when_none():
    """`<field>_isUsed:1` marks a field as optional. Optionals are keyword-only
    (separated from required positional args by `*`) and only appear in the
    output dict when the caller passes a non-None value.
    """
    header = """
    struct ns_Sample {
        int32_t required;
        int32_t optional;
        unsigned int optional_isUsed:1;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["Sample"]
    # Required-only call → no `optional` key in the output.
    assert ctor(1) == {"required": 1}  # type: ignore[operator]
    # Optional present → key included.
    assert ctor(1, optional=9) == {"required": 1, "optional": 9}  # type: ignore[operator]
    # Optional is keyword-only: positional should fail.
    import pytest
    with pytest.raises(TypeError):
        ctor(1, 9)  # type: ignore[operator]


def test_bytes_field_wraps_bytearray_into_bytes_envelope():
    """An anon ``{ uint8_t bytes[N]; uint16_t bytesLen; } Field;`` member
    accepts a ``bytearray`` and emits ``{"bytes": [...ints], "bytesLen": N}``.
    Required and optional bytes fields both wrap.
    """
    header = """
    struct ns_Blob {
        struct {
            uint8_t bytes[ns_Body_BYTES_SIZE];
            uint16_t bytesLen;
        } Body;
        struct {
            uint8_t bytes[ns_Aux_BYTES_SIZE];
            uint16_t bytesLen;
        } Aux;
        unsigned int Aux_isUsed:1;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["Blob"]
    assert ctor(bytearray(b"abc")) == {  # type: ignore[operator]
        "Body": {"bytes": [97, 98, 99], "bytesLen": 3}
    }
    assert ctor(bytearray(b"a"), Aux=bytearray(b"BC")) == {  # type: ignore[operator]
        "Body": {"bytes": [97], "bytesLen": 1},
        "Aux": {"bytes": [66, 67], "bytesLen": 2},
    }


def test_characters_field_wraps_str_into_characters_envelope():
    """An anon ``{ char characters[N]; uint16_t charactersLen; } Field;``
    member accepts a ``str`` and emits
    ``{"characters": [ord(c)...], "charactersLen": N}``.
    Required and optional character fields both wrap.
    """
    header = """
    struct ns_Note {
        struct {
            char characters[ns_Title_CHARACTER_SIZE];
            uint16_t charactersLen;
        } Title;
        struct {
            char characters[ns_Tag_CHARACTER_SIZE];
            uint16_t charactersLen;
        } Tag;
        unsigned int Tag_isUsed:1;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["Note"]
    assert ctor("AB") == {  # type: ignore[operator]
        "Title": {"characters": [65, 66], "charactersLen": 2}
    }
    assert ctor("X", Tag="YZ") == {  # type: ignore[operator]
        "Title": {"characters": [88], "charactersLen": 1},
        "Tag": {"characters": [89, 90], "charactersLen": 2},
    }


def test_enum_typed_scalar_extracts_value_attribute():
    """When a scalar field's C type is one of the header's typedef enums, the
    constructor takes an instance of the (prefix-stripped) enum class and
    emits its ``.value`` into the dict — matching the hand-written
    convention in `V2Gjson/iso2.py`.
    """
    header = """
    typedef enum {
        ns_responseCodeType_OK = 0,
        ns_responseCodeType_FAIL = 99
    } ns_responseCodeType;
    struct ns_Reply {
        ns_responseCodeType ResponseCode;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    Reply = ns["Reply"]
    responseCodeType = ns["responseCodeType"]
    assert Reply(responseCodeType.OK) == {"ResponseCode": 0}  # type: ignore[operator]
    assert Reply(responseCodeType.FAIL) == {"ResponseCode": 99}  # type: ignore[operator]


def test_element_name_casing_is_preserved_verbatim():
    """Top-level element structs in libcbv2g use the element's XSD name
    verbatim, which is lowercase-first for elements like
    ``supportedAppProtocolReq``. The emitter must NOT normalize these to
    PascalCase — `from V2Gjson.sap import supportedAppProtocolReq` is the
    public surface we ship per ADR-0012.
    """
    header = """
    struct appHand_supportedAppProtocolReq {
        int16_t priority;
    };
    """
    src = emit(header, namespace_prefix="appHand_", module_doc="x")
    assert "def supportedAppProtocolReq(" in src
    assert "def SupportedAppProtocolReq(" not in src
    ns = _exec(src)
    assert callable(ns["supportedAppProtocolReq"])


def test_exiDocument_struct_is_skipped():
    """Per ADR-0012, the flat top-level Document shape needs no
    ``exiDocument(...)`` wrapper. The hand-written V2Gjson constructors
    declared one but nothing called it; the emitter must not regress that
    by re-introducing it.
    """
    header = """
    struct ns_exiDocument {
        int16_t Body;
    };
    struct ns_RealType {
        int16_t Value;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    assert "def exiDocument(" not in src
    ns = _exec(src)
    assert "exiDocument" not in ns
    assert callable(ns["RealType"])


def test_empty_struct_emits_zero_arg_constructor_returning_empty_dict():
    """libcbv2g represents semantically-empty XSD complexTypes (e.g. DIN's
    ``IntervalType``, ``EVSEStatusType``) as ``struct { int _unused; }``.
    The parser drops ``_unused``; the emitter follows up with a zero-arg
    constructor that returns ``{}`` so every Namespace struct has a
    constructor available to callers.
    """
    header = """
    struct ns_IntervalType {
        int _unused;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["IntervalType"]
    assert ctor() == {}  # type: ignore[operator]


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
