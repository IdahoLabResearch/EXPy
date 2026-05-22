import textwrap

from codegen.emitter import emit_get_doc
from codegen.parser import TypeSpec, Field


def test_required_scalars_only():
    spec = TypeSpec(
        name="din_AllRequired",
        fields=[
            Field(name="a", c_type="uint32_t"),
            Field(name="b", c_type="int16_t"),
        ],
    )
    expected = textwrap.dedent(
        """\
        struct din_AllRequired getDoc_AllRequired(const json& AllRequiredJson) {
            struct din_AllRequired outDoc;
            init_din_AllRequired(&outDoc);

            outDoc.a = AllRequiredJson["a"].template get<uint32_t>();
            outDoc.b = AllRequiredJson["b"].template get<int16_t>();

            return outDoc;
        }
        """
    )
    assert emit_get_doc(spec) == expected


def test_optional_scalar_sets_is_used_in_both_branches():
    spec = TypeSpec(
        name="din_RelativeTimeIntervalType",
        fields=[
            Field(name="start", c_type="uint32_t"),
            Field(name="duration", c_type="uint32_t", optional=True),
        ],
    )
    expected = textwrap.dedent(
        """\
        struct din_RelativeTimeIntervalType getDoc_RelativeTimeIntervalType(const json& RelativeTimeIntervalTypeJson) {
            struct din_RelativeTimeIntervalType outDoc;
            init_din_RelativeTimeIntervalType(&outDoc);

            outDoc.start = RelativeTimeIntervalTypeJson["start"].template get<uint32_t>();
            if (RelativeTimeIntervalTypeJson.contains("duration")) {
                outDoc.duration_isUsed = 1;
                outDoc.duration = RelativeTimeIntervalTypeJson["duration"].template get<uint32_t>();
            } else {
                outDoc.duration_isUsed = 0;
            }

            return outDoc;
        }
        """
    )
    assert emit_get_doc(spec) == expected


def test_required_characters_field():
    spec = TypeSpec(
        name="din_X",
        fields=[
            Field(name="Algorithm", kind="characters", size_macro="din_Algorithm_CHARACTER_SIZE")
        ],
    )
    expected = textwrap.dedent(
        """\
        struct din_X getDoc_X(const json& XJson) {
            struct din_X outDoc;
            init_din_X(&outDoc);

            outDoc.Algorithm.charactersLen = XJson["Algorithm"]["charactersLen"].template get<uint16_t>();
            for (uint16_t i = 0; i < outDoc.Algorithm.charactersLen && i < din_Algorithm_CHARACTER_SIZE; i++) {
                outDoc.Algorithm.characters[i] = XJson["Algorithm"]["characters"][i].template get<char>();
            }

            return outDoc;
        }
        """
    )
    assert emit_get_doc(spec) == expected


def test_optional_bytes_field():
    spec = TypeSpec(
        name="din_X",
        fields=[
            Field(
                name="ANY",
                kind="bytes",
                size_macro="din_anyType_BYTES_SIZE",
                optional=True,
            )
        ],
    )
    expected = textwrap.dedent(
        """\
        struct din_X getDoc_X(const json& XJson) {
            struct din_X outDoc;
            init_din_X(&outDoc);

            if (XJson.contains("ANY")) {
                outDoc.ANY_isUsed = 1;
                outDoc.ANY.bytesLen = XJson["ANY"]["bytesLen"].template get<uint16_t>();
                for (uint16_t i = 0; i < outDoc.ANY.bytesLen && i < din_anyType_BYTES_SIZE; i++) {
                    outDoc.ANY.bytes[i] = XJson["ANY"]["bytes"][i].template get<uint8_t>();
                }
            } else {
                outDoc.ANY_isUsed = 0;
            }

            return outDoc;
        }
        """
    )
    assert emit_get_doc(spec) == expected


def test_optional_nested_struct_field():
    spec = TypeSpec(
        name="din_ConsumptionCostType",
        fields=[
            Field(name="Cost", kind="struct", c_type="din_CostType", optional=True)
        ],
    )
    expected = textwrap.dedent(
        """\
        struct din_ConsumptionCostType getDoc_ConsumptionCostType(const json& ConsumptionCostTypeJson) {
            struct din_ConsumptionCostType outDoc;
            init_din_ConsumptionCostType(&outDoc);

            if (ConsumptionCostTypeJson.contains("Cost")) {
                outDoc.Cost_isUsed = 1;
                outDoc.Cost = getDoc_CostType(ConsumptionCostTypeJson["Cost"]);
            } else {
                outDoc.Cost_isUsed = 0;
            }

            return outDoc;
        }
        """
    )
    assert emit_get_doc(spec) == expected


def test_required_struct_array_field():
    spec = TypeSpec(
        name="din_SalesTariffType",
        fields=[
            Field(
                name="SalesTariffEntry",
                kind="array",
                c_type="din_SalesTariffEntryType",
                size_macro="din_SalesTariffEntryType_5_ARRAY_SIZE",
            )
        ],
    )
    expected = textwrap.dedent(
        """\
        struct din_SalesTariffType getDoc_SalesTariffType(const json& SalesTariffTypeJson) {
            struct din_SalesTariffType outDoc;
            init_din_SalesTariffType(&outDoc);

            outDoc.SalesTariffEntry.arrayLen = SalesTariffTypeJson["SalesTariffEntry"]["arrayLen"].template get<uint16_t>();
            for (uint16_t i = 0; i < outDoc.SalesTariffEntry.arrayLen && i < din_SalesTariffEntryType_5_ARRAY_SIZE; i++) {
                outDoc.SalesTariffEntry.array[i] = getDoc_SalesTariffEntryType(SalesTariffTypeJson["SalesTariffEntry"]["array"][i]);
            }

            return outDoc;
        }
        """
    )
    assert emit_get_doc(spec) == expected
