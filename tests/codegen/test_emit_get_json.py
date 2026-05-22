"""Emit getJson_ for scalar-only structs, with and without optional fields."""

import textwrap

from codegen.emitter import emit_get_json
from codegen.parser import TypeSpec, Field


def test_emits_get_json_for_required_scalars_only():
    spec = TypeSpec(
        name="din_AllRequired",
        fields=[
            Field(name="a", c_type="uint32_t"),
            Field(name="b", c_type="int16_t"),
        ],
    )

    expected = textwrap.dedent(
        """\
        json getJson_AllRequired(const struct din_AllRequired& AllRequiredDoc) {
            json outJson = json::object();

            outJson["a"] = AllRequiredDoc.a;
            outJson["b"] = AllRequiredDoc.b;

            return outJson;
        }
        """
    )

    assert emit_get_json(spec) == expected


def test_emits_is_used_guard_for_optional_scalar():
    spec = TypeSpec(
        name="din_RelativeTimeIntervalType",
        fields=[
            Field(name="start", c_type="uint32_t"),
            Field(name="duration", c_type="uint32_t", optional=True),
        ],
    )

    expected = textwrap.dedent(
        """\
        json getJson_RelativeTimeIntervalType(const struct din_RelativeTimeIntervalType& RelativeTimeIntervalTypeDoc) {
            json outJson = json::object();

            outJson["start"] = RelativeTimeIntervalTypeDoc.start;
            if (RelativeTimeIntervalTypeDoc.duration_isUsed) {
                outJson["duration"] = RelativeTimeIntervalTypeDoc.duration;
            }

            return outJson;
        }
        """
    )

    assert emit_get_json(spec) == expected
