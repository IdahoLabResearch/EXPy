"""Tracer-bullet test: parse one all-scalar struct from a minimal header."""

from codegen.parser import parse_header


HEADER_FRAGMENT = """
struct din_RelativeTimeIntervalType {
    // start, unsignedInt (base: unsignedLong)
    uint32_t start;
    // duration, unsignedInt (base: unsignedLong)
    uint32_t duration;
    unsigned int duration_isUsed:1;

};
"""


def test_parses_struct_name_and_required_field():
    [spec] = parse_header(HEADER_FRAGMENT)

    assert spec.name == "din_RelativeTimeIntervalType"
    assert spec.fields[0].name == "start"
    assert spec.fields[0].c_type == "uint32_t"
    assert spec.fields[0].optional is False


def test_parses_optional_field_marked_by_is_used_bitfield():
    [spec] = parse_header(HEADER_FRAGMENT)

    duration = spec.fields[1]
    assert duration.name == "duration"
    assert duration.c_type == "uint32_t"
    assert duration.optional is True


def test_parses_bitfield_scalar():
    """`exi_signed_t.is_negative` is declared as `uint8_t is_negative : 1;`.
    The bitfield suffix is irrelevant on the JSON side — it just round-trips
    as a 0/1 int. The parser should treat it as a plain scalar.
    """
    [spec] = parse_header("""
        struct exi_signed_t {
            uint8_t is_negative : 1;
        };
    """)
    [f] = spec.fields
    assert f.name == "is_negative"
    assert f.kind == "scalar"
    assert f.c_type == "uint8_t"


def test_parses_raw_byte_array_field():
    """`exi_unsigned_t.octets` is the bare shape `uint8_t octets[N];` —
    NOT the `{ bytes[N]; bytesLen; }` envelope. The parser must classify
    it as a distinct field kind so the emitter can produce the
    ``{"octets": list(...)}`` shape without an arrayLen wrapper.
    """
    [spec] = parse_header("""
        typedef struct {
            uint8_t octets[EXI_BASETYPES_MAX_OCTETS_SUPPORTED];
            size_t octets_count;
        } exi_unsigned_t;
    """)
    octets = spec.fields[0]
    assert octets.name == "octets"
    assert octets.kind == "raw_bytes"
    assert octets.size_macro == "EXI_BASETYPES_MAX_OCTETS_SUPPORTED"
    count = spec.fields[1]
    assert count.name == "octets_count"
    assert count.kind == "scalar"


def test_parses_anonymous_typedef_struct():
    """`exi_basetypes.h` declares `exi_unsigned_t` via the anonymous form
    `typedef struct { ... } name;` (no struct tag). The parser must emit a
    TypeSpec for it the same way it does for named structs.
    """
    [spec] = parse_header("""
        typedef struct {
            uint32_t count;
        } anon_typedef_t;
    """)
    assert spec.name == "anon_typedef_t"
    assert [(f.name, f.c_type) for f in spec.fields] == [("count", "uint32_t")]
