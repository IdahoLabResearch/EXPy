"""Parser recognizes all field kinds emitted by libcbv2g's generator."""

from codegen.parser import parse_header


def _only(header: str):
    return parse_header(header)[0]


def test_character_array_field():
    spec = _only("""
        struct din_X {
            struct {
                char characters[din_Algorithm_CHARACTER_SIZE];
                uint16_t charactersLen;
            } Algorithm;
        };
    """)
    [f] = spec.fields
    assert f.name == "Algorithm"
    assert f.kind == "characters"
    assert f.size_macro == "din_Algorithm_CHARACTER_SIZE"
    assert f.optional is False


def test_bytes_array_field():
    spec = _only("""
        struct din_X {
            struct {
                uint8_t bytes[din_anyType_BYTES_SIZE];
                uint16_t bytesLen;
            } ANY;
            unsigned int ANY_isUsed:1;
        };
    """)
    [f] = spec.fields
    assert f.name == "ANY"
    assert f.kind == "bytes"
    assert f.size_macro == "din_anyType_BYTES_SIZE"
    assert f.optional is True


def test_nested_struct_field():
    spec = _only("""
        struct din_X {
            struct din_CostType Cost;
            unsigned int Cost_isUsed:1;
        };
    """)
    [f] = spec.fields
    assert f.name == "Cost"
    assert f.kind == "struct"
    assert f.c_type == "din_CostType"
    assert f.optional is True


def test_nested_struct_array_field():
    spec = _only("""
        struct din_X {
            struct {
                struct din_SalesTariffEntryType array[din_SalesTariffEntryType_5_ARRAY_SIZE];
                uint16_t arrayLen;
            } SalesTariffEntry;
        };
    """)
    [f] = spec.fields
    assert f.name == "SalesTariffEntry"
    assert f.kind == "array"
    assert f.c_type == "din_SalesTariffEntryType"
    assert f.size_macro == "din_SalesTariffEntryType_5_ARRAY_SIZE"


def test_empty_struct_with_unused_field_is_skipped():
    """`int _unused;` is libcbv2g's placeholder for semantically empty types;
    the marshaler should treat the struct as fieldless."""
    spec = _only("""
        struct din_IntervalType {
            int _unused;
        };
    """)
    assert spec.fields == []


def test_scalar_field_kind_defaults():
    spec = _only("""
        struct din_X {
            uint32_t start;
        };
    """)
    [f] = spec.fields
    assert f.kind == "scalar"
    assert f.c_type == "uint32_t"
