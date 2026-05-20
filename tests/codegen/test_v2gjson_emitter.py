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


def test_bytes_array_wraps_each_element_into_bytes_envelope():
    """``{ struct { uint8_t bytes[N]; uint16_t bytesLen; } array[M]; uint16_t arrayLen; }``
    — the wire shape is a list of byte-envelope dicts. Constructor takes
    ``list[bytearray]`` and emits each element as ``{"bytes": [...], "bytesLen": N}``,
    matching the hand-written ``X509DataType.SubCertificates`` shape.
    """
    header = """
    struct ns_SubCertificatesType {
        struct {
            struct {
                uint8_t bytes[ns_Certificate_BYTES_SIZE];
                uint16_t bytesLen;
            } array[ns_Certificate_ARRAY_SIZE];
            uint16_t arrayLen;
        } Certificate;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["SubCertificatesType"]
    certs = [bytearray(b"AB"), bytearray(b"X")]
    assert ctor(certs) == {  # type: ignore[operator]
        "Certificate": {
            "arrayLen": 2,
            "array": [
                {"bytes": [65, 66], "bytesLen": 2},
                {"bytes": [88], "bytesLen": 1},
            ],
        }
    }


def test_chars_array_wraps_each_element_into_characters_envelope():
    """``{ struct { char characters[N]; uint16_t charactersLen; } array[M]; uint16_t arrayLen; }``
    — list of character-envelope dicts. Constructor takes ``list[str]`` and
    emits ``{"characters": [ord(c)...], "charactersLen": N}`` per element.
    Matches DIN's ``ListOfRootCertificateIDsType.RootCertificateID``.
    """
    header = """
    struct ns_ListOfRootCertificateIDsType {
        struct {
            struct {
                char characters[ns_RootCertificateID_CHARACTER_SIZE];
                uint16_t charactersLen;
            } array[ns_RootCertificateID_ARRAY_SIZE];
            uint16_t arrayLen;
        } RootCertificateID;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["ListOfRootCertificateIDsType"]
    got = ctor(["AB", "X"])  # type: ignore[operator]
    assert got == {
        "RootCertificateID": {
            "arrayLen": 2,
            "array": [
                {"characters": [65, 66], "charactersLen": 2},
                {"characters": [88], "charactersLen": 1},
            ],
        }
    }


def test_scalar_array_of_enum_emits_value_unwrapping():
    """``{ <enumType> array[N]; uint16_t arrayLen; }`` — the wire shape carries
    raw ints, so the constructor takes a ``list[<EnumClass>]`` and emits
    ``{"arrayLen": len(x), "array": [v.value for v in x]}``. This matches the
    hand-written ``DIN_PaymentOptionsType`` shape exactly.
    """
    header = """
    typedef enum {
        ns_paymentOptionType_Contract = 0,
        ns_paymentOptionType_External = 1
    } ns_paymentOptionType;
    struct ns_PaymentOptionsType {
        struct {
            ns_paymentOptionType array[ns_PaymentOption_ARRAY_SIZE];
            uint16_t arrayLen;
        } PaymentOption;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["PaymentOptionsType"]
    Enum = ns["paymentOptionType"]
    got = ctor([Enum.Contract, Enum.External])  # type: ignore[operator]
    assert got == {"PaymentOption": {"arrayLen": 2, "array": [0, 1]}}


def test_scalar_array_of_plain_int_emits_list_verbatim():
    """``{ uint8_t array[N]; uint16_t arrayLen; }`` — non-enum scalar array.
    Constructor takes a ``list[int]`` and emits the raw ints inside the
    arrayLen envelope.
    """
    header = """
    struct ns_Bag {
        struct {
            uint8_t array[ns_Items_ARRAY_SIZE];
            uint16_t arrayLen;
        } Items;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["Bag"]
    assert ctor([3, 5, 7]) == {  # type: ignore[operator]
        "Items": {"arrayLen": 3, "array": [3, 5, 7]}
    }


def test_struct_array_field_wraps_into_arrayLen_envelope():
    """An anon ``{ struct ns_Entry array[N]; uint16_t arrayLen; } Field;``
    member accepts a ``list[dict]`` and emits the EVerest array shape:
    ``{"arrayLen": len(x), "array": x}`` — matching what the marshaler reads.
    The hand-written V2Gjson constructors wrapped lists this way; the codegen
    output must do the same so the public API stays callable for list fields.
    """
    header = """
    struct ns_Container {
        struct {
            struct ns_Entry array[ns_Entry_ARRAY_SIZE];
            uint16_t arrayLen;
        } Entries;
    };
    """
    src = emit(header, namespace_prefix="ns_", module_doc="x")
    ns = _exec(src)
    ctor = ns["Container"]
    entries = [{"k": 1}, {"k": 2}]
    assert ctor(entries) == {  # type: ignore[operator]
        "Entries": {"arrayLen": 2, "array": entries}
    }
