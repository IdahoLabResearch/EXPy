import textwrap

from codegen.emitter import emit_get_json
from codegen.parser import TypeSpec, Field


def test_required_bytes_field():
    spec = TypeSpec(
        name="din_X",
        fields=[
            Field(name="evccID", kind="bytes", size_macro="din_evccIDType_BYTES_SIZE")
        ],
    )
    expected = textwrap.dedent(
        """\
        json getJson_X(const struct din_X& XDoc) {
            json outJson = json::object();

            outJson["evccID"]["bytesLen"] = XDoc.evccID.bytesLen;
            for (uint16_t i = 0; i < XDoc.evccID.bytesLen && i < din_evccIDType_BYTES_SIZE; i++) {
                outJson["evccID"]["bytes"][i] = XDoc.evccID.bytes[i];
            }

            return outJson;
        }
        """
    )
    assert emit_get_json(spec) == expected


def test_optional_nested_struct_field():
    spec = TypeSpec(
        name="din_ConsumptionCostType",
        fields=[
            Field(name="Cost", kind="struct", c_type="din_CostType", optional=True),
        ],
    )
    expected = textwrap.dedent(
        """\
        json getJson_ConsumptionCostType(const struct din_ConsumptionCostType& ConsumptionCostTypeDoc) {
            json outJson = json::object();

            if (ConsumptionCostTypeDoc.Cost_isUsed) {
                outJson["Cost"] = getJson_CostType(ConsumptionCostTypeDoc.Cost);
            }

            return outJson;
        }
        """
    )
    assert emit_get_json(spec) == expected


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
        json getJson_SalesTariffType(const struct din_SalesTariffType& SalesTariffTypeDoc) {
            json outJson = json::object();

            outJson["SalesTariffEntry"]["arrayLen"] = SalesTariffTypeDoc.SalesTariffEntry.arrayLen;
            for (uint16_t i = 0; i < SalesTariffTypeDoc.SalesTariffEntry.arrayLen && i < din_SalesTariffEntryType_5_ARRAY_SIZE; i++) {
                outJson["SalesTariffEntry"]["array"][i] = getJson_SalesTariffEntryType(SalesTariffTypeDoc.SalesTariffEntry.array[i]);
            }

            return outJson;
        }
        """
    )
    assert emit_get_json(spec) == expected
