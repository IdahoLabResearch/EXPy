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
