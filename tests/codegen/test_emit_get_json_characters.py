import textwrap

from codegen.emitter import emit_get_json
from codegen.parser import TypeSpec, Field


def test_required_characters_field():
    spec = TypeSpec(
        name="din_X",
        fields=[
            Field(
                name="Algorithm",
                kind="characters",
                size_macro="din_Algorithm_CHARACTER_SIZE",
            )
        ],
    )
    expected = textwrap.dedent(
        """\
        json getJson_X(const struct din_X& XDoc) {
            json outJson = json::object();

            outJson["Algorithm"]["charactersLen"] = XDoc.Algorithm.charactersLen;
            for (uint16_t i = 0; i < XDoc.Algorithm.charactersLen && i < din_Algorithm_CHARACTER_SIZE; i++) {
                outJson["Algorithm"]["characters"][i] = XDoc.Algorithm.characters[i];
            }

            return outJson;
        }
        """
    )
    assert emit_get_json(spec) == expected


def test_optional_characters_field():
    spec = TypeSpec(
        name="din_X",
        fields=[
            Field(
                name="XPath",
                kind="characters",
                size_macro="din_XPath_CHARACTER_SIZE",
                optional=True,
            )
        ],
    )
    expected = textwrap.dedent(
        """\
        json getJson_X(const struct din_X& XDoc) {
            json outJson = json::object();

            if (XDoc.XPath_isUsed) {
                outJson["XPath"]["charactersLen"] = XDoc.XPath.charactersLen;
                for (uint16_t i = 0; i < XDoc.XPath.charactersLen && i < din_XPath_CHARACTER_SIZE; i++) {
                    outJson["XPath"]["characters"][i] = XDoc.XPath.characters[i];
                }
            }

            return outJson;
        }
        """
    )
    assert emit_get_json(spec) == expected
