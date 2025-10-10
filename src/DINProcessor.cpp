/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <new>

#include <cbv2g/din/din_msgDefDecoder.h>
#include <cbv2g/din/din_msgDefEncoder.h>
#include <cbv2g/din/din_msgDefDatatypes.h>

#include <common.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <cxxopts.hpp>

#include <iostream>
#include <fstream>
using namespace std;

// TODO: Check and enforce maximum sizes of arrays and strings (e.g., TransformType.Algorithm_CHARACTER_SIZE)
json getJson_CostType(const struct din_CostType& CostTypeDoc) {
    json outJson;

    outJson["costKind"] = CostTypeDoc.costKind;
    outJson["amount"] = CostTypeDoc.amount;
    if (CostTypeDoc.amountMultiplier_isUsed) {
        outJson["amountMultiplier"] = CostTypeDoc.amountMultiplier;
    }
    return outJson;
}

struct din_CostType getDoc_CostType(const json& CostTypeJson) {
    struct din_CostType outDoc;
    init_din_CostType(&outDoc);

    outDoc.costKind = CostTypeJson["costKind"].template get<din_costKindType>();
    outDoc.amount = CostTypeJson["amount"].template get<uint32_t>();
    if (CostTypeJson.contains("amountMultiplier")) {
        outDoc.amountMultiplier_isUsed = 1;
        outDoc.amountMultiplier = CostTypeJson["amountMultiplier"].template get<int8_t>();
    } else {
        outDoc.amountMultiplier_isUsed = 0;
    }

    return outDoc;
}

json getJson_RelativeTimeIntervalType(const struct din_RelativeTimeIntervalType& RelativeTimeIntervalTypeDoc) {
    json outJson;

    outJson["start"] = RelativeTimeIntervalTypeDoc.start;
    if (RelativeTimeIntervalTypeDoc.duration_isUsed) {
        outJson["duration"] = RelativeTimeIntervalTypeDoc.duration;
    }

    return outJson;
}

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

json getJson_IntervalType(const struct din_IntervalType& IntervalTypeDoc) {
    json outJson;

    outJson["_unused"] = IntervalTypeDoc._unused;

    return outJson;
}

struct din_IntervalType getDoc_IntervalType(const json& IntervalTypeJson) {
    struct din_IntervalType outDoc;
    init_din_IntervalType(&outDoc);

    outDoc._unused = IntervalTypeJson["_unused"].template get<int>();

    return outDoc;
}

json getJson_ConsumptionCostType(const struct din_ConsumptionCostType& ConsumptionCostTypeDoc) {
    json outJson;

    outJson["startValue"] = ConsumptionCostTypeDoc.startValue;
    if (ConsumptionCostTypeDoc.Cost_isUsed) {
        outJson["Cost"] = getJson_CostType(ConsumptionCostTypeDoc.Cost);
    }

    return outJson;
}

struct din_ConsumptionCostType getDoc_ConsumptionCostType(const json& ConsumptionCostTypeJson) {
    struct din_ConsumptionCostType outDoc;
    init_din_ConsumptionCostType(&outDoc);

    outDoc.startValue = ConsumptionCostTypeJson["startValue"].template get<uint32_t>();
    if (ConsumptionCostTypeJson.contains("Cost")) {
        outDoc.Cost_isUsed = 1;
        outDoc.Cost = getDoc_CostType(ConsumptionCostTypeJson["Cost"]);
    } else {
        outDoc.Cost_isUsed = 0;
    }

    return outDoc;
}

json getJson_TransformType(const struct din_TransformType& TransformTypeDoc) {
    json outJson;

    outJson["Algorithm"]["charactersLen"] = TransformTypeDoc.Algorithm.charactersLen;
    for (uint16_t i = 0; i < TransformTypeDoc.Algorithm.charactersLen; i++) {
        outJson["Algorithm"]["characters"][i] = TransformTypeDoc.Algorithm.characters[i];
    }
    if (TransformTypeDoc.ANY_isUsed) {
        outJson["ANY"]["bytesLen"] = TransformTypeDoc.ANY.bytesLen;
        for (uint16_t i = 0; i < TransformTypeDoc.ANY.bytesLen; i++) {
            outJson["ANY"]["bytes"][i] = TransformTypeDoc.ANY.bytes[i];
        }
    }
    if (TransformTypeDoc.XPath_isUsed) {
        outJson["XPath"]["charactersLen"] = TransformTypeDoc.XPath.charactersLen;
        for (uint16_t i = 0; i < TransformTypeDoc.XPath.charactersLen; i++) {
            outJson["XPath"]["characters"][i] = TransformTypeDoc.XPath.characters[i];
        }
    }

    return outJson;
}

struct din_TransformType getDoc_TransformType(const json& TransformTypeJson) {
    struct din_TransformType outDoc;
    init_din_TransformType(&outDoc);

    outDoc.Algorithm.charactersLen = TransformTypeJson["Algorithm"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Algorithm.charactersLen; i++) {
        outDoc.Algorithm.characters[i] = TransformTypeJson["Algorithm"]["characters"][i].template get<char>();
    }
    if (TransformTypeJson.contains("ANY")) {
        outDoc.ANY_isUsed = 1;
        outDoc.ANY.bytesLen = TransformTypeJson["ANY"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ANY.bytesLen; i++) {
            outDoc.ANY.bytes[i] = TransformTypeJson["ANY"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.ANY_isUsed = 0;
    }
    if (TransformTypeJson.contains("XPath")) {
        outDoc.XPath_isUsed = 1;
        outDoc.XPath.charactersLen = TransformTypeJson["XPath"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.XPath.charactersLen; i++) {
            outDoc.XPath.characters[i] = TransformTypeJson["XPath"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.XPath_isUsed = 0;
    }

    return outDoc;
}

json getJson_PMaxScheduleEntryType(const struct din_PMaxScheduleEntryType& PMaxScheduleEntryTypeDoc) {
    json outJson;

    if (PMaxScheduleEntryTypeDoc.RelativeTimeInterval_isUsed) {
        outJson["RelativeTimeInterval"] = getJson_RelativeTimeIntervalType(PMaxScheduleEntryTypeDoc.RelativeTimeInterval);
    }
    if (PMaxScheduleEntryTypeDoc.TimeInterval_isUsed) {
        outJson["TimeInterval"] = getJson_IntervalType(PMaxScheduleEntryTypeDoc.TimeInterval);
    }
    outJson["PMax"] = PMaxScheduleEntryTypeDoc.PMax;

    return outJson;
}

struct din_PMaxScheduleEntryType getDoc_PMaxScheduleEntryType(const json& PMaxScheduleEntryTypeJson) {
    struct din_PMaxScheduleEntryType outDoc;
    init_din_PMaxScheduleEntryType(&outDoc);

    if (PMaxScheduleEntryTypeJson.contains("RelativeTimeInterval")) {
        outDoc.RelativeTimeInterval_isUsed = 1;
        outDoc.RelativeTimeInterval = getDoc_RelativeTimeIntervalType(PMaxScheduleEntryTypeJson["RelativeTimeInterval"]);
    } else {
        outDoc.RelativeTimeInterval_isUsed = 0;
    }
    if (PMaxScheduleEntryTypeJson.contains("TimeInterval")) {
        outDoc.TimeInterval_isUsed = 1;
        outDoc.TimeInterval = getDoc_IntervalType(PMaxScheduleEntryTypeJson["TimeInterval"]);
    } else {
        outDoc.TimeInterval_isUsed = 0;
    }
    outDoc.PMax = PMaxScheduleEntryTypeJson["PMax"].template get<int16_t>();

    return outDoc;
}

json getJson_SalesTariffEntryType(const struct din_SalesTariffEntryType& SalesTariffEntryTypeDoc) {
    json outJson;

    if (SalesTariffEntryTypeDoc.RelativeTimeInterval_isUsed) {
        outJson["RelativeTimeInterval"] = getJson_RelativeTimeIntervalType(SalesTariffEntryTypeDoc.RelativeTimeInterval);
    }
    if (SalesTariffEntryTypeDoc.TimeInterval_isUsed) {
        outJson["TimeInterval"] = getJson_IntervalType(SalesTariffEntryTypeDoc.TimeInterval);
    }
    outJson["EPriceLevel"] = SalesTariffEntryTypeDoc.EPriceLevel;
    if (SalesTariffEntryTypeDoc.ConsumptionCost_isUsed) {
        outJson["ConsumptionCost"] = getJson_ConsumptionCostType(SalesTariffEntryTypeDoc.ConsumptionCost);
    }

    return outJson;
}

struct din_SalesTariffEntryType getDoc_SalesTariffEntryType(const json& SalesTariffEntryTypeJson) {
    struct din_SalesTariffEntryType outDoc;
    init_din_SalesTariffEntryType(&outDoc);

    if (SalesTariffEntryTypeJson.contains("RelativeTimeInterval")) {
        outDoc.RelativeTimeInterval_isUsed = 1;
        outDoc.RelativeTimeInterval = getDoc_RelativeTimeIntervalType(SalesTariffEntryTypeJson["RelativeTimeInterval"]);
    } else {
        outDoc.RelativeTimeInterval_isUsed = 0;
    }
    if (SalesTariffEntryTypeJson.contains("TimeInterval")) {
        outDoc.TimeInterval_isUsed = 1;
        outDoc.TimeInterval = getDoc_IntervalType(SalesTariffEntryTypeJson["TimeInterval"]);
    } else {
        outDoc.TimeInterval_isUsed = 0;
    }
    outDoc.EPriceLevel = SalesTariffEntryTypeJson["EPriceLevel"].template get<uint8_t>();
    if (SalesTariffEntryTypeJson.contains("ConsumptionCost")) {
        outDoc.ConsumptionCost_isUsed = 1;
        outDoc.ConsumptionCost = getDoc_ConsumptionCostType(SalesTariffEntryTypeJson["ConsumptionCost"]);
    } else {
        outDoc.ConsumptionCost_isUsed = 0;
    }

    return outDoc;
}

json getJson_TransformsType(const struct din_TransformsType& TransformsTypeDoc) {
    json outJson;

    outJson["Transform"] = getJson_TransformType(TransformsTypeDoc.Transform);

    return outJson;
}

struct din_TransformsType getDoc_TransformsType(const json& TransformsTypeJson) {
    struct din_TransformsType outDoc;
    init_din_TransformsType(&outDoc);

    outDoc.Transform = getDoc_TransformType(TransformsTypeJson["Transform"]);

    return outDoc;
}

json getJson_DSAKeyValueType(const struct din_DSAKeyValueType& DSAKeyValueTypeDoc) {
    json outJson;

    if (DSAKeyValueTypeDoc.P_isUsed) {
        outJson["P"]["bytesLen"] = DSAKeyValueTypeDoc.P.bytesLen;
        for (uint16_t i = 0; i < DSAKeyValueTypeDoc.P.bytesLen; i++) {
            outJson["P"]["bytes"][i] = DSAKeyValueTypeDoc.P.bytes[i];
        }
    }
    if (DSAKeyValueTypeDoc.Q_isUsed) {
        outJson["Q"]["bytesLen"] = DSAKeyValueTypeDoc.Q.bytesLen;
        for (uint16_t i = 0; i < DSAKeyValueTypeDoc.Q.bytesLen; i++) {
            outJson["Q"]["bytes"][i] = DSAKeyValueTypeDoc.Q.bytes[i];
        }
    }
    if (DSAKeyValueTypeDoc.G_isUsed) {
        outJson["G"]["bytesLen"] = DSAKeyValueTypeDoc.G.bytesLen;
        for (uint16_t i = 0; i < DSAKeyValueTypeDoc.G.bytesLen; i++) {
            outJson["G"]["bytes"][i] = DSAKeyValueTypeDoc.G.bytes[i];
        }
    }
    outJson["Y"]["bytesLen"] = DSAKeyValueTypeDoc.Y.bytesLen;
    for (uint16_t i = 0; i < DSAKeyValueTypeDoc.Y.bytesLen; i++) {
        outJson["Y"]["bytes"][i] = DSAKeyValueTypeDoc.Y.bytes[i];
    }
    if (DSAKeyValueTypeDoc.J_isUsed) {
        outJson["J"]["bytesLen"] = DSAKeyValueTypeDoc.J.bytesLen;
        for (uint16_t i = 0; i < DSAKeyValueTypeDoc.J.bytesLen; i++) {
            outJson["J"]["bytes"][i] = DSAKeyValueTypeDoc.J.bytes[i];
        }
    }
    if (DSAKeyValueTypeDoc.Seed_isUsed) {
        outJson["Seed"]["bytesLen"] = DSAKeyValueTypeDoc.Seed.bytesLen;
        for (uint16_t i = 0; i < DSAKeyValueTypeDoc.Seed.bytesLen; i++) {
            outJson["Seed"]["bytes"][i] = DSAKeyValueTypeDoc.Seed.bytes[i];
        }
    }
    if (DSAKeyValueTypeDoc.PgenCounter_isUsed) {
        outJson["PgenCounter"]["bytesLen"] = DSAKeyValueTypeDoc.PgenCounter.bytesLen;
        for (uint16_t i = 0; i < DSAKeyValueTypeDoc.PgenCounter.bytesLen; i++) {
            outJson["PgenCounter"]["bytes"][i] = DSAKeyValueTypeDoc.PgenCounter.bytes[i];
        }
    }

    return outJson;
}

struct din_DSAKeyValueType getDoc_DSAKeyValueType(const json& DSAKeyValueTypeJson) {
    struct din_DSAKeyValueType outDoc;
    init_din_DSAKeyValueType(&outDoc);

    if (DSAKeyValueTypeJson.contains("P")) {
        outDoc.P_isUsed = 1;
        outDoc.P.bytesLen = DSAKeyValueTypeJson["P"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.P.bytesLen; i++) {
            outDoc.P.bytes[i] = DSAKeyValueTypeJson["P"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.P_isUsed = 0;
    }
    if (DSAKeyValueTypeJson.contains("Q")) {
        outDoc.Q_isUsed = 1;
        outDoc.Q.bytesLen = DSAKeyValueTypeJson["Q"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Q.bytesLen; i++) {
            outDoc.Q.bytes[i] = DSAKeyValueTypeJson["Q"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.Q_isUsed = 0;
    }
    if (DSAKeyValueTypeJson.contains("G")) {
        outDoc.G_isUsed = 1;
        outDoc.G.bytesLen = DSAKeyValueTypeJson["G"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.G.bytesLen; i++) {
            outDoc.G.bytes[i] = DSAKeyValueTypeJson["G"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.G_isUsed = 0;
    }
    outDoc.Y.bytesLen = DSAKeyValueTypeJson["Y"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Y.bytesLen; i++) {
        outDoc.Y.bytes[i] = DSAKeyValueTypeJson["Y"]["bytes"][i].template get<uint8_t>();
    }
    if (DSAKeyValueTypeJson.contains("J")) {
        outDoc.J_isUsed = 1;
        outDoc.J.bytesLen = DSAKeyValueTypeJson["J"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.J.bytesLen; i++) {
            outDoc.J.bytes[i] = DSAKeyValueTypeJson["J"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.J_isUsed = 0;
    }
    if (DSAKeyValueTypeJson.contains("Seed")) {
        outDoc.Seed_isUsed = 1;
        outDoc.Seed.bytesLen = DSAKeyValueTypeJson["Seed"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Seed.bytesLen; i++) {
            outDoc.Seed.bytes[i] = DSAKeyValueTypeJson["Seed"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.Seed_isUsed = 0;
    }
    if (DSAKeyValueTypeJson.contains("PgenCounter")) {
        outDoc.PgenCounter_isUsed = 1;
        outDoc.PgenCounter.bytesLen = DSAKeyValueTypeJson["PgenCounter"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.PgenCounter.bytesLen; i++) {
            outDoc.PgenCounter.bytes[i] = DSAKeyValueTypeJson["PgenCounter"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.PgenCounter_isUsed = 0;
    }

    return outDoc;
}

json getJson_X509IssuerSerialType(const struct din_X509IssuerSerialType& X509IssuerSerialTypeDoc) {
    json outJson;

    outJson["X509IssuerName"]["charactersLen"] = X509IssuerSerialTypeDoc.X509IssuerName.charactersLen;
    for (uint16_t i = 0; i < X509IssuerSerialTypeDoc.X509IssuerName.charactersLen; i++) {
        outJson["X509IssuerName"]["characters"][i] = X509IssuerSerialTypeDoc.X509IssuerName.characters[i];
    }
    outJson["X509SerialNumber"] = getJson_exi_signed_t(X509IssuerSerialTypeDoc.X509SerialNumber);

    return outJson;
}

struct din_X509IssuerSerialType getDoc_X509IssuerSerialType(const json& X509IssuerSerialTypeJson) {
    struct din_X509IssuerSerialType outDoc;
    init_din_X509IssuerSerialType(&outDoc);

    outDoc.X509IssuerName.charactersLen = X509IssuerSerialTypeJson["X509IssuerName"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.X509IssuerName.charactersLen; i++) {
        outDoc.X509IssuerName.characters[i] = X509IssuerSerialTypeJson["X509IssuerName"]["characters"][i].template get<char>();
    }
    outDoc.X509SerialNumber = getDoc_exi_signed_t(X509IssuerSerialTypeJson["X509SerialNumber"]);

    return outDoc;
}

json getJson_DigestMethodType(const struct din_DigestMethodType& DigestMethodTypeDoc) {
    json outJson;

    outJson["Algorithm"]["charactersLen"] = DigestMethodTypeDoc.Algorithm.charactersLen;
    for (uint16_t i = 0; i < DigestMethodTypeDoc.Algorithm.charactersLen; i++) {
        outJson["Algorithm"]["characters"][i] = DigestMethodTypeDoc.Algorithm.characters[i];
    }
    if (DigestMethodTypeDoc.ANY_isUsed) {
        outJson["ANY"]["bytesLen"] = DigestMethodTypeDoc.ANY.bytesLen;
        for (uint16_t i = 0; i < DigestMethodTypeDoc.ANY.bytesLen; i++) {
            outJson["ANY"]["bytes"][i] = DigestMethodTypeDoc.ANY.bytes[i];
        }
    }

    return outJson;
}

struct din_DigestMethodType getDoc_DigestMethodType(const json& DigestMethodTypeJson) {
    struct din_DigestMethodType outDoc;
    init_din_DigestMethodType(&outDoc);

    outDoc.Algorithm.charactersLen = DigestMethodTypeJson["Algorithm"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Algorithm.charactersLen; i++) {
        outDoc.Algorithm.characters[i] = DigestMethodTypeJson["Algorithm"]["characters"][i].template get<char>();
    }
    if (DigestMethodTypeJson.contains("ANY")) {
        outDoc.ANY_isUsed = 1;
        outDoc.ANY.bytesLen = DigestMethodTypeJson["ANY"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ANY.bytesLen; i++) {
            outDoc.ANY.bytes[i] = DigestMethodTypeJson["ANY"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.ANY_isUsed = 0;
    }

    return outDoc;
}

json getJson_RSAKeyValueType(const struct din_RSAKeyValueType& RSAKeyValueTypeDoc) {
    json outJson;

    outJson["Modulus"]["bytesLen"] = RSAKeyValueTypeDoc.Modulus.bytesLen;
    for (uint16_t i = 0; i < RSAKeyValueTypeDoc.Modulus.bytesLen; i++) {
        outJson["Modulus"]["bytes"][i] = RSAKeyValueTypeDoc.Modulus.bytes[i];
    }
    outJson["Exponent"]["bytesLen"] = RSAKeyValueTypeDoc.Exponent.bytesLen;
    for (uint16_t i = 0; i < RSAKeyValueTypeDoc.Exponent.bytesLen; i++) {
        outJson["Exponent"]["bytes"][i] = RSAKeyValueTypeDoc.Exponent.bytes[i];
    }

    return outJson;
}

struct din_RSAKeyValueType getDoc_RSAKeyValueType(const json& RSAKeyValueTypeJson) {
    struct din_RSAKeyValueType outDoc;
    init_din_RSAKeyValueType(&outDoc);

    outDoc.Modulus.bytesLen = RSAKeyValueTypeJson["Modulus"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Modulus.bytesLen; i++) {
        outDoc.Modulus.bytes[i] = RSAKeyValueTypeJson["Modulus"]["bytes"][i].template get<uint8_t>();
    }
    outDoc.Exponent.bytesLen = RSAKeyValueTypeJson["Exponent"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Exponent.bytesLen; i++) {
        outDoc.Exponent.bytes[i] = RSAKeyValueTypeJson["Exponent"]["bytes"][i].template get<uint8_t>();
    }

    return outDoc;
}

json getJson_PMaxScheduleType(const struct din_PMaxScheduleType& PMaxScheduleTypeDoc) {
    json outJson;

    outJson["PMaxScheduleID"] = PMaxScheduleTypeDoc.PMaxScheduleID;
    outJson["PMaxScheduleEntry"]["arrayLen"] = PMaxScheduleTypeDoc.PMaxScheduleEntry.arrayLen;
    for (uint16_t i = 0; i < PMaxScheduleTypeDoc.PMaxScheduleEntry.arrayLen; i++) {
        outJson["PMaxScheduleEntry"]["array"][i] = getJson_PMaxScheduleEntryType(PMaxScheduleTypeDoc.PMaxScheduleEntry.array[i]);
    }

    return outJson;
}

struct din_PMaxScheduleType getDoc_PMaxScheduleType(const json& PMaxScheduleTypeJson) {
    struct din_PMaxScheduleType outDoc;
    init_din_PMaxScheduleType(&outDoc);

    outDoc.PMaxScheduleID = PMaxScheduleTypeJson["PMaxScheduleID"].template get<uint8_t>();
    outDoc.PMaxScheduleEntry.arrayLen = PMaxScheduleTypeJson["PMaxScheduleEntry"]["arrayLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.PMaxScheduleEntry.arrayLen; i++) {
        outDoc.PMaxScheduleEntry.array[i] = getDoc_PMaxScheduleEntryType(PMaxScheduleTypeJson["PMaxScheduleEntry"]["array"][i]);
    }

    return outDoc;
}

json getJson_SalesTariffType(const struct din_SalesTariffType& SalesTariffTypeDoc) {
    json outJson;

    outJson["Id"]["charactersLen"] = SalesTariffTypeDoc.Id.charactersLen;
    for (uint16_t i = 0; i < SalesTariffTypeDoc.Id.charactersLen; i++) {
        outJson["Id"]["characters"][i] = SalesTariffTypeDoc.Id.characters[i];
    }
    outJson["SalesTariffID"] = SalesTariffTypeDoc.SalesTariffID;
    if (SalesTariffTypeDoc.SalesTariffDescription_isUsed) {
        outJson["SalesTariffDescription"]["charactersLen"] = SalesTariffTypeDoc.SalesTariffDescription.charactersLen;
        for (uint16_t i = 0; i < SalesTariffTypeDoc.SalesTariffDescription.charactersLen; i++) {
            outJson["SalesTariffDescription"]["characters"][i] = SalesTariffTypeDoc.SalesTariffDescription.characters[i];
        }
    }
    outJson["NumEPriceLevels"] = SalesTariffTypeDoc.NumEPriceLevels;
    outJson["SalesTariffEntry"]["arrayLen"] = SalesTariffTypeDoc.SalesTariffEntry.arrayLen;
    for (uint16_t i = 0; i < SalesTariffTypeDoc.SalesTariffEntry.arrayLen; i++) {
        outJson["SalesTariffEntry"]["array"][i] = getJson_SalesTariffEntryType(SalesTariffTypeDoc.SalesTariffEntry.array[i]);
    }

    return outJson;
}

struct din_SalesTariffType getDoc_SalesTariffType(const json& SalesTariffTypeJson) {
    struct din_SalesTariffType outDoc;
    init_din_SalesTariffType(&outDoc);

    outDoc.Id.charactersLen = SalesTariffTypeJson["Id"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
        outDoc.Id.characters[i] = SalesTariffTypeJson["Id"]["characters"][i].template get<char>();
    }
    outDoc.SalesTariffID = SalesTariffTypeJson["SalesTariffID"].template get<int16_t>();
    if (SalesTariffTypeJson.contains("SalesTariffDescription")) {
        outDoc.SalesTariffDescription_isUsed = 1;
        outDoc.SalesTariffDescription.charactersLen = SalesTariffTypeJson["SalesTariffDescription"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.SalesTariffDescription.charactersLen; i++) {
            outDoc.SalesTariffDescription.characters[i] = SalesTariffTypeJson["SalesTariffDescription"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.SalesTariffDescription_isUsed = 0;
    }
    outDoc.NumEPriceLevels = SalesTariffTypeJson["NumEPriceLevels"].template get<uint8_t>();
    outDoc.SalesTariffEntry.arrayLen = SalesTariffTypeJson["SalesTariffEntry"]["arrayLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.SalesTariffEntry.arrayLen; i++) {
        outDoc.SalesTariffEntry.array[i] = getDoc_SalesTariffEntryType(SalesTariffTypeJson["SalesTariffEntry"]["array"][i]);
    }

    return outDoc;
}

json getJson_CanonicalizationMethodType(const struct din_CanonicalizationMethodType& CanonicalizationMethodTypeDoc) {
    json outJson;

    outJson["Algorithm"]["charactersLen"] = CanonicalizationMethodTypeDoc.Algorithm.charactersLen;
    for (uint16_t i = 0; i < CanonicalizationMethodTypeDoc.Algorithm.charactersLen; i++) {
        outJson["Algorithm"]["characters"][i] = CanonicalizationMethodTypeDoc.Algorithm.characters[i];
    }
    if (CanonicalizationMethodTypeDoc.ANY_isUsed) {
        outJson["ANY"]["bytesLen"] = CanonicalizationMethodTypeDoc.ANY.bytesLen;
        for (uint16_t i = 0; i < CanonicalizationMethodTypeDoc.ANY.bytesLen; i++) {
            outJson["ANY"]["bytes"][i] = CanonicalizationMethodTypeDoc.ANY.bytes[i];
        }
    }

    return outJson;
}

struct din_CanonicalizationMethodType getDoc_CanonicalizationMethodType(const json& CanonicalizationMethodTypeJson) {
    struct din_CanonicalizationMethodType outDoc;
    init_din_CanonicalizationMethodType(&outDoc);

    outDoc.Algorithm.charactersLen = CanonicalizationMethodTypeJson["Algorithm"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Algorithm.charactersLen; i++) {
        outDoc.Algorithm.characters[i] = CanonicalizationMethodTypeJson["Algorithm"]["characters"][i].template get<char>();
    }
    if (CanonicalizationMethodTypeJson.contains("ANY")) {
        outDoc.ANY_isUsed = 1;
        outDoc.ANY.bytesLen = CanonicalizationMethodTypeJson["ANY"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ANY.bytesLen; i++) {
            outDoc.ANY.bytes[i] = CanonicalizationMethodTypeJson["ANY"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.ANY_isUsed = 0;
    }

    return outDoc;
}

json getJson_ServiceTagType(const struct din_ServiceTagType& ServiceTagTypeDoc) {
    json outJson;

    outJson["ServiceID"] = ServiceTagTypeDoc.ServiceID;
    if (ServiceTagTypeDoc.ServiceName_isUsed) {
        outJson["ServiceName"]["charactersLen"] = ServiceTagTypeDoc.ServiceName.charactersLen;
        for (uint16_t i = 0; i < ServiceTagTypeDoc.ServiceName.charactersLen; i++) {
            outJson["ServiceName"]["characters"][i] = ServiceTagTypeDoc.ServiceName.characters[i];
        }
    }
    outJson["ServiceCategory"] = ServiceTagTypeDoc.ServiceCategory;
    if (ServiceTagTypeDoc.ServiceScope_isUsed) {
        outJson["ServiceScope"]["charactersLen"] = ServiceTagTypeDoc.ServiceScope.charactersLen;
        for (uint16_t i = 0; i < ServiceTagTypeDoc.ServiceScope.charactersLen; i++) {
            outJson["ServiceScope"]["characters"][i] = ServiceTagTypeDoc.ServiceScope.characters[i];
        }
    }

    return outJson;
}

struct din_ServiceTagType getDoc_ServiceTagType(const json& ServiceTagTypeJson) {
    struct din_ServiceTagType outDoc;
    init_din_ServiceTagType(&outDoc);

    outDoc.ServiceID = ServiceTagTypeJson["ServiceID"].template get<uint16_t>();
    if (ServiceTagTypeJson.contains("ServiceName")) {
        outDoc.ServiceName_isUsed = 1;
        outDoc.ServiceName.charactersLen = ServiceTagTypeJson["ServiceName"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ServiceName.charactersLen; i++) {
            outDoc.ServiceName.characters[i] = ServiceTagTypeJson["ServiceName"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.ServiceName_isUsed = 0;
    }
    outDoc.ServiceCategory = ServiceTagTypeJson["ServiceCategory"].template get<din_serviceCategoryType>();
    if (ServiceTagTypeJson.contains("ServiceScope")) {
        outDoc.ServiceScope_isUsed = 1;
        outDoc.ServiceScope.charactersLen = ServiceTagTypeJson["ServiceScope"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ServiceScope.charactersLen; i++) {
            outDoc.ServiceScope.characters[i] = ServiceTagTypeJson["ServiceScope"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.ServiceScope_isUsed = 0;
    }

    return outDoc;
}

json getJson_ServiceType(const struct din_ServiceType& ServiceTypeDoc) {
    json outJson;

    outJson["ServiceTag"] = getJson_ServiceTagType(ServiceTypeDoc.ServiceTag);
    outJson["FreeService"] = ServiceTypeDoc.FreeService;

    return outJson;
}

struct din_ServiceType getDoc_ServiceType(const json& ServiceTypeJson) {
    struct din_ServiceType outDoc;
    init_din_ServiceType(&outDoc);

    outDoc.ServiceTag = getDoc_ServiceTagType(ServiceTypeJson["ServiceTag"]);
    outDoc.FreeService = ServiceTypeJson["FreeService"].template get<int>();

    return outDoc;
}

json getJson_SelectedServiceType(const struct din_SelectedServiceType& SelectedServiceTypeDoc) {
    json outJson;

    outJson["ServiceID"] = SelectedServiceTypeDoc.ServiceID;
    if (SelectedServiceTypeDoc.ParameterSetID_isUsed) {
        outJson["ParameterSetID"] = SelectedServiceTypeDoc.ParameterSetID;
    }

    return outJson;
}

struct din_SelectedServiceType getDoc_SelectedServiceType(const json& SelectedServiceTypeJson) {
    struct din_SelectedServiceType outDoc;
    init_din_SelectedServiceType(&outDoc);

    outDoc.ServiceID = SelectedServiceTypeJson["ServiceID"].template get<uint16_t>();
    if (SelectedServiceTypeJson.contains("ParameterSetID")) {
        outDoc.ParameterSetID_isUsed = 1;
        outDoc.ParameterSetID = SelectedServiceTypeJson["ParameterSetID"].template get<int16_t>();
    } else {
        outDoc.ParameterSetID_isUsed = 0;
    }

    return outDoc;
}

json getJson_SAScheduleTupleType(const struct din_SAScheduleTupleType& SAScheduleTupleTypeDoc) {
    json outJson;

    outJson["SAScheduleTupleID"] = SAScheduleTupleTypeDoc.SAScheduleTupleID;
    outJson["PMaxSchedule"] = getJson_PMaxScheduleType(SAScheduleTupleTypeDoc.PMaxSchedule);
    if (SAScheduleTupleTypeDoc.SalesTariff_isUsed) {
        outJson["SalesTariff"] = getJson_SalesTariffType(SAScheduleTupleTypeDoc.SalesTariff);
    }

    return outJson;
}

struct din_SAScheduleTupleType getDoc_SAScheduleTupleType(const json& SAScheduleTupleTypeJson) {
    struct din_SAScheduleTupleType outDoc;
    init_din_SAScheduleTupleType(&outDoc);

    outDoc.SAScheduleTupleID = SAScheduleTupleTypeJson["SAScheduleTupleID"].template get<int16_t>();
    outDoc.PMaxSchedule = getDoc_PMaxScheduleType(SAScheduleTupleTypeJson["PMaxSchedule"]);
    if (SAScheduleTupleTypeJson.contains("SalesTariff")) {
        outDoc.SalesTariff_isUsed = 1;
        outDoc.SalesTariff = getDoc_SalesTariffType(SAScheduleTupleTypeJson["SalesTariff"]);
    } else {
        outDoc.SalesTariff_isUsed = 0;
    }

    return outDoc;
}

json getJson_AC_EVSEStatusType(const struct din_AC_EVSEStatusType& AC_EVSEStatusTypeDoc) {
    json outJson;

    outJson["PowerSwitchClosed"] = AC_EVSEStatusTypeDoc.PowerSwitchClosed;
    outJson["RCD"] = AC_EVSEStatusTypeDoc.RCD;
    outJson["NotificationMaxDelay"] = AC_EVSEStatusTypeDoc.NotificationMaxDelay;
    outJson["EVSENotification"] = AC_EVSEStatusTypeDoc.EVSENotification;

    return outJson;
}

struct din_AC_EVSEStatusType getDoc_AC_EVSEStatusType(const json& AC_EVSEStatusTypeJson) {
    struct din_AC_EVSEStatusType outDoc;
    init_din_AC_EVSEStatusType(&outDoc);

    outDoc.PowerSwitchClosed = AC_EVSEStatusTypeJson["PowerSwitchClosed"].template get<int>();
    outDoc.RCD = AC_EVSEStatusTypeJson["RCD"].template get<int>();
    outDoc.NotificationMaxDelay = AC_EVSEStatusTypeJson["NotificationMaxDelay"].template get<uint32_t>();
    outDoc.EVSENotification = AC_EVSEStatusTypeJson["EVSENotification"].template get<din_EVSENotificationType>();

    return outDoc;
}

json getJson_SignatureMethodType(const struct din_SignatureMethodType& SignatureMethodTypeDoc) {
    json outJson;

    outJson["Algorithm"]["charactersLen"] = SignatureMethodTypeDoc.Algorithm.charactersLen;
    for (uint16_t i = 0; i < SignatureMethodTypeDoc.Algorithm.charactersLen; i++) {
        outJson["Algorithm"]["characters"][i] = SignatureMethodTypeDoc.Algorithm.characters[i];
    }
    if (SignatureMethodTypeDoc.HMACOutputLength_isUsed) {
        outJson["HMACOutputLength"] = getJson_exi_signed_t(SignatureMethodTypeDoc.HMACOutputLength);
    }
    if (SignatureMethodTypeDoc.ANY_isUsed) {
        outJson["ANY"]["bytesLen"] = SignatureMethodTypeDoc.ANY.bytesLen;
        for (uint16_t i = 0; i < SignatureMethodTypeDoc.ANY.bytesLen; i++) {
            outJson["ANY"]["bytes"][i] = SignatureMethodTypeDoc.ANY.bytes[i];
        }
    }

    return outJson;
}

struct din_SignatureMethodType getDoc_SignatureMethodType(const json& SignatureMethodTypeJson) {
    struct din_SignatureMethodType outDoc;
    init_din_SignatureMethodType(&outDoc);

    outDoc.Algorithm.charactersLen = SignatureMethodTypeJson["Algorithm"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Algorithm.charactersLen; i++) {
        outDoc.Algorithm.characters[i] = SignatureMethodTypeJson["Algorithm"]["characters"][i].template get<char>();
    }
    if (SignatureMethodTypeJson.contains("HMACOutputLength")) {
        outDoc.HMACOutputLength_isUsed = 1;
        outDoc.HMACOutputLength = getDoc_exi_signed_t(SignatureMethodTypeJson["HMACOutputLength"]);
    } else {
        outDoc.HMACOutputLength_isUsed = 0;
    }
    if (SignatureMethodTypeJson.contains("ANY")) {
        outDoc.ANY_isUsed = 1;
        outDoc.ANY.bytesLen = SignatureMethodTypeJson["ANY"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ANY.bytesLen; i++) {
            outDoc.ANY.bytes[i] = SignatureMethodTypeJson["ANY"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.ANY_isUsed = 0;
    }

    return outDoc;
}

json getJson_KeyValueType(const struct din_KeyValueType& KeyValueTypeDoc) {
    json outJson;

    if (KeyValueTypeDoc.RSAKeyValue_isUsed) {
        outJson["RSAKeyValue"] = getJson_RSAKeyValueType(KeyValueTypeDoc.RSAKeyValue);
    }
    if (KeyValueTypeDoc.DSAKeyValue_isUsed) {
        outJson["DSAKeyValue"] = getJson_DSAKeyValueType(KeyValueTypeDoc.DSAKeyValue);
    }
    if (KeyValueTypeDoc.ANY_isUsed) {
        outJson["ANY"]["bytesLen"] = KeyValueTypeDoc.ANY.bytesLen;
        for (uint16_t i = 0; i < KeyValueTypeDoc.ANY.bytesLen; i++) {
            outJson["ANY"]["bytes"][i] = KeyValueTypeDoc.ANY.bytes[i];
        }
    }

    return outJson;
}

struct din_KeyValueType getDoc_KeyValueType(const json& KeyValueTypeJson) {
    struct din_KeyValueType outDoc;
    init_din_KeyValueType(&outDoc);

    if (KeyValueTypeJson.contains("RSAKeyValue")) {
        outDoc.RSAKeyValue_isUsed = 1;
        outDoc.RSAKeyValue = getDoc_RSAKeyValueType(KeyValueTypeJson["RSAKeyValue"]);
    } else {
        outDoc.RSAKeyValue_isUsed = 0;
    }
    if (KeyValueTypeJson.contains("DSAKeyValue")) {
        outDoc.DSAKeyValue_isUsed = 1;
        outDoc.DSAKeyValue = getDoc_DSAKeyValueType(KeyValueTypeJson["DSAKeyValue"]);
    } else {
        outDoc.DSAKeyValue_isUsed = 0;
    }
    if (KeyValueTypeJson.contains("ANY")) {
        outDoc.ANY_isUsed = 1;
        outDoc.ANY.bytesLen = KeyValueTypeJson["ANY"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ANY.bytesLen; i++) {
            outDoc.ANY.bytes[i] = KeyValueTypeJson["ANY"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.ANY_isUsed = 0;
    }

    return outDoc;
}

json getJson_SubCertificatesType(const struct din_SubCertificatesType& SubCertificatesTypeDoc) {
    json outJson;

    outJson["Certificate"]["bytesLen"] = SubCertificatesTypeDoc.Certificate.bytesLen;
    for (uint16_t i = 0; i < SubCertificatesTypeDoc.Certificate.bytesLen; i++) {
        outJson["Certificate"]["bytes"][i] = SubCertificatesTypeDoc.Certificate.bytes[i];
    }

    return outJson;
}

struct din_SubCertificatesType getDoc_SubCertificatesType(const json& SubCertificatesTypeJson) {
    struct din_SubCertificatesType outDoc;
    init_din_SubCertificatesType(&outDoc);

    outDoc.Certificate.bytesLen = SubCertificatesTypeJson["Certificate"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Certificate.bytesLen; i++) {
        outDoc.Certificate.bytes[i] = SubCertificatesTypeJson["Certificate"]["bytes"][i].template get<uint8_t>();
    }

    return outDoc;
}

json getJson_ProfileEntryType(const struct din_ProfileEntryType& ProfileEntryTypeDoc) {
    json outJson;

    outJson["ChargingProfileEntryStart"] = ProfileEntryTypeDoc.ChargingProfileEntryStart;
    outJson["ChargingProfileEntryMaxPower"] = ProfileEntryTypeDoc.ChargingProfileEntryMaxPower;

    return outJson;
}

struct din_ProfileEntryType getDoc_ProfileEntryType(const json& ProfileEntryTypeJson) {
    struct din_ProfileEntryType outDoc;
    init_din_ProfileEntryType(&outDoc);

    outDoc.ChargingProfileEntryStart = ProfileEntryTypeJson["ChargingProfileEntryStart"].template get<uint32_t>();
    outDoc.ChargingProfileEntryMaxPower = ProfileEntryTypeJson["ChargingProfileEntryMaxPower"].template get<int16_t>();

    return outDoc;
}

json getJson_ReferenceType(const struct din_ReferenceType& ReferenceTypeDoc) {
    json outJson;

    if (ReferenceTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = ReferenceTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < ReferenceTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = ReferenceTypeDoc.Id.characters[i];
        }
    }
    if (ReferenceTypeDoc.Type_isUsed) {
        outJson["Type"]["charactersLen"] = ReferenceTypeDoc.Type.charactersLen;
        for (uint16_t i = 0; i < ReferenceTypeDoc.Type.charactersLen; i++) {
            outJson["Type"]["characters"][i] = ReferenceTypeDoc.Type.characters[i];
        }
    }
    if (ReferenceTypeDoc.URI_isUsed) {
        outJson["URI"]["charactersLen"] = ReferenceTypeDoc.URI.charactersLen;
        for (uint16_t i = 0; i < ReferenceTypeDoc.URI.charactersLen; i++) {
            outJson["URI"]["characters"][i] = ReferenceTypeDoc.URI.characters[i];
        }
    }
    if (ReferenceTypeDoc.Transforms_isUsed) {
        outJson["Transforms"] = getJson_TransformsType(ReferenceTypeDoc.Transforms);
    }
    outJson["DigestMethod"] = getJson_DigestMethodType(ReferenceTypeDoc.DigestMethod);
    outJson["DigestValue"]["bytesLen"] = ReferenceTypeDoc.DigestValue.bytesLen;
    for (uint16_t i = 0; i < ReferenceTypeDoc.DigestValue.bytesLen; i++) {
        outJson["DigestValue"]["bytes"][i] = ReferenceTypeDoc.DigestValue.bytes[i];
    }

    return outJson;
}

struct din_ReferenceType getDoc_ReferenceType(const json& ReferenceTypeJson) {
    struct din_ReferenceType outDoc;
    init_din_ReferenceType(&outDoc);

    if (ReferenceTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = ReferenceTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = ReferenceTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    if (ReferenceTypeJson.contains("Type")) {
        outDoc.Type_isUsed = 1;
        outDoc.Type.charactersLen = ReferenceTypeJson["Type"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Type.charactersLen; i++) {
            outDoc.Type.characters[i] = ReferenceTypeJson["Type"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Type_isUsed = 0;
    }
    if (ReferenceTypeJson.contains("URI")) {
        outDoc.URI_isUsed = 1;
        outDoc.URI.charactersLen = ReferenceTypeJson["URI"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.URI.charactersLen; i++) {
            outDoc.URI.characters[i] = ReferenceTypeJson["URI"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.URI_isUsed = 0;
    }
    if (ReferenceTypeJson.contains("Transforms")) {
        outDoc.Transforms_isUsed = 1;
        outDoc.Transforms = getDoc_TransformsType(ReferenceTypeJson["Transforms"]);
    } else {
        outDoc.Transforms_isUsed = 0;
    }
    outDoc.DigestMethod = getDoc_DigestMethodType(ReferenceTypeJson["DigestMethod"]);
    outDoc.DigestValue.bytesLen = ReferenceTypeJson["DigestValue"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.DigestValue.bytesLen; i++) {
        outDoc.DigestValue.bytes[i] = ReferenceTypeJson["DigestValue"]["bytes"][i].template get<uint8_t>();
    }

    return outDoc;
}

json getJson_RetrievalMethodType(const struct din_RetrievalMethodType& RetrievalMethodTypeDoc) {
    json outJson;

    if (RetrievalMethodTypeDoc.Type_isUsed) {
        outJson["Type"]["charactersLen"] = RetrievalMethodTypeDoc.Type.charactersLen;
        for (uint16_t i = 0; i < RetrievalMethodTypeDoc.Type.charactersLen; i++) {
            outJson["Type"]["characters"][i] = RetrievalMethodTypeDoc.Type.characters[i];
        }
    }
    if (RetrievalMethodTypeDoc.URI_isUsed) {
        outJson["URI"]["charactersLen"] = RetrievalMethodTypeDoc.URI.charactersLen;
        for (uint16_t i = 0; i < RetrievalMethodTypeDoc.URI.charactersLen; i++) {
            outJson["URI"]["characters"][i] = RetrievalMethodTypeDoc.URI.characters[i];
        }
    }
    if (RetrievalMethodTypeDoc.Transforms_isUsed) {
        outJson["Transforms"] = getJson_TransformsType(RetrievalMethodTypeDoc.Transforms);
    }

    return outJson;
}

struct din_RetrievalMethodType getDoc_RetrievalMethodType(const json& RetrievalMethodTypeJson) {
    struct din_RetrievalMethodType outDoc;
    init_din_RetrievalMethodType(&outDoc);

    if (RetrievalMethodTypeJson.contains("Type")) {
        outDoc.Type_isUsed = 1;
        outDoc.Type.charactersLen = RetrievalMethodTypeJson["Type"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Type.charactersLen; i++) {
            outDoc.Type.characters[i] = RetrievalMethodTypeJson["Type"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Type_isUsed = 0;
    }
    if (RetrievalMethodTypeJson.contains("URI")) {
        outDoc.URI_isUsed = 1;
        outDoc.URI.charactersLen = RetrievalMethodTypeJson["URI"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.URI.charactersLen; i++) {
            outDoc.URI.characters[i] = RetrievalMethodTypeJson["URI"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.URI_isUsed = 0;
    }
    if (RetrievalMethodTypeJson.contains("Transforms")) {
        outDoc.Transforms_isUsed = 1;
        outDoc.Transforms = getDoc_TransformsType(RetrievalMethodTypeJson["Transforms"]);
    } else {
        outDoc.Transforms_isUsed = 0;
    }

    return outDoc;
}

json getJson_X509DataType(const struct din_X509DataType& X509DataTypeDoc) {
    json outJson;

    if (X509DataTypeDoc.X509IssuerSerial_isUsed) {
        outJson["X509IssuerSerial"] = getJson_X509IssuerSerialType(X509DataTypeDoc.X509IssuerSerial);
    }
    if (X509DataTypeDoc.X509SKI_isUsed) {
        outJson["X509SKI"]["bytesLen"] = X509DataTypeDoc.X509SKI.bytesLen;
        for (uint16_t i = 0; i < X509DataTypeDoc.X509SKI.bytesLen; i++) {
            outJson["X509SKI"]["bytes"][i] = X509DataTypeDoc.X509SKI.bytes[i];
        }
    }
    if (X509DataTypeDoc.X509SubjectName_isUsed) {
        outJson["X509SubjectName"]["charactersLen"] = X509DataTypeDoc.X509SubjectName.charactersLen;
        for (uint16_t i = 0; i < X509DataTypeDoc.X509SubjectName.charactersLen; i++) {
            outJson["X509SubjectName"]["characters"][i] = X509DataTypeDoc.X509SubjectName.characters[i];
        }
    }
    if (X509DataTypeDoc.X509Certificate_isUsed) {
        outJson["X509Certificate"]["bytesLen"] = X509DataTypeDoc.X509Certificate.bytesLen;
        for (uint16_t i = 0; i < X509DataTypeDoc.X509Certificate.bytesLen; i++) {
            outJson["X509Certificate"]["bytes"][i] = X509DataTypeDoc.X509Certificate.bytes[i];
        }
    }
    if (X509DataTypeDoc.X509CRL_isUsed) {
        outJson["X509CRL"]["bytesLen"] = X509DataTypeDoc.X509CRL.bytesLen;
        for (uint16_t i = 0; i < X509DataTypeDoc.X509CRL.bytesLen; i++) {
            outJson["X509CRL"]["bytes"][i] = X509DataTypeDoc.X509CRL.bytes[i];
        }
    }
    if (X509DataTypeDoc.ANY_isUsed) {
        outJson["ANY"]["bytesLen"] = X509DataTypeDoc.ANY.bytesLen;
        for (uint16_t i = 0; i < X509DataTypeDoc.ANY.bytesLen; i++) {
            outJson["ANY"]["bytes"][i] = X509DataTypeDoc.ANY.bytes[i];
        }
    }

    return outJson;
}

struct din_X509DataType getDoc_X509DataType(const json& X509DataTypeJson) {
    struct din_X509DataType outDoc;
    init_din_X509DataType(&outDoc);

    if (X509DataTypeJson.contains("X509IssuerSerial")) {
        outDoc.X509IssuerSerial_isUsed = 1;
        outDoc.X509IssuerSerial = getDoc_X509IssuerSerialType(X509DataTypeJson["X509IssuerSerial"]);
    } else {
        outDoc.X509IssuerSerial_isUsed = 0;
    }
    if (X509DataTypeJson.contains("X509SKI")) {
        outDoc.X509SKI_isUsed = 1;
        outDoc.X509SKI.bytesLen = X509DataTypeJson["X509SKI"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.X509SKI.bytesLen; i++) {
            outDoc.X509SKI.bytes[i] = X509DataTypeJson["X509SKI"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.X509SKI_isUsed = 0;
    }
    if (X509DataTypeJson.contains("X509SubjectName")) {
        outDoc.X509SubjectName_isUsed = 1;
        outDoc.X509SubjectName.charactersLen = X509DataTypeJson["X509SubjectName"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.X509SubjectName.charactersLen; i++) {
            outDoc.X509SubjectName.characters[i] = X509DataTypeJson["X509SubjectName"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.X509SubjectName_isUsed = 0;
    }
    if (X509DataTypeJson.contains("X509Certificate")) {
        outDoc.X509Certificate_isUsed = 1;
        outDoc.X509Certificate.bytesLen = X509DataTypeJson["X509Certificate"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.X509Certificate.bytesLen; i++) {
            outDoc.X509Certificate.bytes[i] = X509DataTypeJson["X509Certificate"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.X509Certificate_isUsed = 0;
    }
    if (X509DataTypeJson.contains("X509CRL")) {
        outDoc.X509CRL_isUsed = 1;
        outDoc.X509CRL.bytesLen = X509DataTypeJson["X509CRL"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.X509CRL.bytesLen; i++) {
            outDoc.X509CRL.bytes[i] = X509DataTypeJson["X509CRL"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.X509CRL_isUsed = 0;
    }
    if (X509DataTypeJson.contains("ANY")) {
        outDoc.ANY_isUsed = 1;
        outDoc.ANY.bytesLen = X509DataTypeJson["ANY"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ANY.bytesLen; i++) {
            outDoc.ANY.bytes[i] = X509DataTypeJson["ANY"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.ANY_isUsed = 0;
    }

    return outDoc;
}

json getJson_PGPDataType(const struct din_PGPDataType& PGPDataTypeDoc) {
    json outJson;

    if (PGPDataTypeDoc.choice_1_isUsed) {
        outJson["choice_1"]["PGPKeyID"]["bytesLen"] = PGPDataTypeDoc.choice_1.PGPKeyID.bytesLen;
        for (uint16_t i = 0; i < PGPDataTypeDoc.choice_1.PGPKeyID.bytesLen; i++) {
            outJson["choice_1"]["PGPKeyID"]["bytes"][i] = PGPDataTypeDoc.choice_1.PGPKeyID.bytes[i];
        }
        if (PGPDataTypeDoc.choice_1.PGPKeyPacket_isUsed) {
            outJson["choice_1"]["PGPKeyPacket"]["bytesLen"] = PGPDataTypeDoc.choice_1.PGPKeyPacket.bytesLen;
            for (uint16_t i = 0; i < PGPDataTypeDoc.choice_1.PGPKeyPacket.bytesLen; i++) {
                outJson["choice_1"]["PGPKeyPacket"]["bytes"][i] = PGPDataTypeDoc.choice_1.PGPKeyPacket.bytes[i];
            }
        }
        if (PGPDataTypeDoc.choice_1.ANY_isUsed) {
            outJson["choice_1"]["ANY"]["bytesLen"] = PGPDataTypeDoc.choice_1.ANY.bytesLen;
            for (uint16_t i = 0; i < PGPDataTypeDoc.choice_1.ANY.bytesLen; i++) {
                outJson["choice_1"]["ANY"]["bytes"][i] = PGPDataTypeDoc.choice_1.ANY.bytes[i];
            }
        }
    }
    if (PGPDataTypeDoc.choice_2_isUsed) {
        outJson["choice_2"]["PGPKeyPacket"]["bytesLen"] = PGPDataTypeDoc.choice_2.PGPKeyPacket.bytesLen;
        for (uint16_t i = 0; i < PGPDataTypeDoc.choice_2.PGPKeyPacket.bytesLen; i++) {
            outJson["choice_2"]["PGPKeyPacket"]["bytes"][i] = PGPDataTypeDoc.choice_2.PGPKeyPacket.bytes[i];
        }
        if (PGPDataTypeDoc.choice_2.ANY_isUsed) {
            outJson["choice_2"]["ANY"]["bytesLen"] = PGPDataTypeDoc.choice_2.ANY.bytesLen;
            for (uint16_t i = 0; i < PGPDataTypeDoc.choice_2.ANY.bytesLen; i++) {
                outJson["choice_2"]["ANY"]["bytes"][i] = PGPDataTypeDoc.choice_2.ANY.bytes[i];
            }
        }
    }

    return outJson;
}

struct din_PGPDataType getDoc_PGPDataType(const json& PGPDataTypeJson) {
    struct din_PGPDataType outDoc;
    init_din_PGPDataType(&outDoc);

    if (PGPDataTypeJson.contains("choice_1")) {
        outDoc.choice_1_isUsed = 1;
        outDoc.choice_1.PGPKeyID.bytesLen = PGPDataTypeJson["choice_1"]["PGPKeyID"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.choice_1.PGPKeyID.bytesLen; i++) {
            outDoc.choice_1.PGPKeyID.bytes[i] = PGPDataTypeJson["choice_1"]["PGPKeyID"]["bytes"][i].template get<uint8_t>();
        }
        if (PGPDataTypeJson["choice_1"].contains("PGPKeyPacket")) {
            outDoc.choice_1.PGPKeyPacket_isUsed = 1;
            outDoc.choice_1.PGPKeyPacket.bytesLen = PGPDataTypeJson["choice_1"]["PGPKeyPacket"]["bytesLen"].template get<uint16_t>();
            for (uint16_t i = 0; i < outDoc.choice_1.PGPKeyPacket.bytesLen; i++) {
                outDoc.choice_1.PGPKeyPacket.bytes[i] = PGPDataTypeJson["choice_1"]["PGPKeyPacket"]["bytes"][i].template get<uint8_t>();
            }
        } else {
            outDoc.choice_1.PGPKeyPacket_isUsed = 0;
        }
        if (PGPDataTypeJson["choice_1"].contains("ANY")) {
            outDoc.choice_1.ANY_isUsed = 1;
            outDoc.choice_1.ANY.bytesLen = PGPDataTypeJson["choice_1"]["ANY"]["bytesLen"].template get<uint16_t>();
            for (uint16_t i = 0; i < outDoc.choice_1.ANY.bytesLen; i++) {
                outDoc.choice_1.ANY.bytes[i] = PGPDataTypeJson["choice_1"]["ANY"]["bytes"][i].template get<uint8_t>();
            }
        } else {
            outDoc.choice_1.ANY_isUsed = 0;
        }
    } else {
        outDoc.choice_1_isUsed = 0;
    }
    if (PGPDataTypeJson.contains("choice_2")) {
        outDoc.choice_2_isUsed = 1;
        outDoc.choice_2.PGPKeyPacket.bytesLen = PGPDataTypeJson["choice_2"]["PGPKeyPacket"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.choice_2.PGPKeyPacket.bytesLen; i++) {
            outDoc.choice_2.PGPKeyPacket.bytes[i] = PGPDataTypeJson["choice_2"]["PGPKeyPacket"]["bytes"][i].template get<uint8_t>();
        }
        if (PGPDataTypeJson["choice_2"].contains("ANY")) {
            outDoc.choice_2.ANY_isUsed = 1;
            outDoc.choice_2.ANY.bytesLen = PGPDataTypeJson["choice_2"]["ANY"]["bytesLen"].template get<uint16_t>();
            for (uint16_t i = 0; i < outDoc.choice_2.ANY.bytesLen; i++) {
                outDoc.choice_2.ANY.bytes[i] = PGPDataTypeJson["choice_2"]["ANY"]["bytes"][i].template get<uint8_t>();
            }
        } else {
            outDoc.choice_2.ANY_isUsed = 0;
        }
    } else {
        outDoc.choice_2_isUsed = 0;
    }

    return outDoc;
}

json getJson_SPKIDataType(const struct din_SPKIDataType& SPKIDataTypeDoc) {
    json outJson;

    outJson["SPKISexp"]["bytesLen"] = SPKIDataTypeDoc.SPKISexp.bytesLen;
    for (uint16_t i = 0; i < SPKIDataTypeDoc.SPKISexp.bytesLen; i++) {
        outJson["SPKISexp"]["bytes"][i] = SPKIDataTypeDoc.SPKISexp.bytes[i];
    }
    if (SPKIDataTypeDoc.ANY_isUsed) {
        outJson["ANY"]["bytesLen"] = SPKIDataTypeDoc.ANY.bytesLen;
        for (uint16_t i = 0; i < SPKIDataTypeDoc.ANY.bytesLen; i++) {
            outJson["ANY"]["bytes"][i] = SPKIDataTypeDoc.ANY.bytes[i];
        }
    }

    return outJson;
}

struct din_SPKIDataType getDoc_SPKIDataType(const json& SPKIDataTypeJson) {
    struct din_SPKIDataType outDoc;
    init_din_SPKIDataType(&outDoc);

    outDoc.SPKISexp.bytesLen = SPKIDataTypeJson["SPKISexp"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.SPKISexp.bytesLen; i++) {
        outDoc.SPKISexp.bytes[i] = SPKIDataTypeJson["SPKISexp"]["bytes"][i].template get<uint8_t>();
    }
    if (SPKIDataTypeJson.contains("ANY")) {
        outDoc.ANY_isUsed = 1;
        outDoc.ANY.bytesLen = SPKIDataTypeJson["ANY"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ANY.bytesLen; i++) {
            outDoc.ANY.bytes[i] = SPKIDataTypeJson["ANY"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.ANY_isUsed = 0;
    }

    return outDoc;
}

json getJson_SignedInfoType(const struct din_SignedInfoType& SignedInfoTypeDoc) {
    json outJson;

    if (SignedInfoTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = SignedInfoTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < SignedInfoTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = SignedInfoTypeDoc.Id.characters[i];
        }
    }
    outJson["CanonicalizationMethod"] = getJson_CanonicalizationMethodType(SignedInfoTypeDoc.CanonicalizationMethod);
    outJson["SignatureMethod"] = getJson_SignatureMethodType(SignedInfoTypeDoc.SignatureMethod);
    outJson["Reference"] = getJson_ReferenceType(SignedInfoTypeDoc.Reference);

    return outJson;
}

struct din_SignedInfoType getDoc_SignedInfoType(const json& SignedInfoTypeJson) {
    struct din_SignedInfoType outDoc;
    init_din_SignedInfoType(&outDoc);

    if (SignedInfoTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = SignedInfoTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = SignedInfoTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    outDoc.CanonicalizationMethod = getDoc_CanonicalizationMethodType(SignedInfoTypeJson["CanonicalizationMethod"]);
    outDoc.SignatureMethod = getDoc_SignatureMethodType(SignedInfoTypeJson["SignatureMethod"]);
    outDoc.Reference = getDoc_ReferenceType(SignedInfoTypeJson["Reference"]);

    return outDoc;
}

json getJson_DC_EVStatusType(const struct din_DC_EVStatusType& DC_EVStatusTypeDoc) {
    json outJson;

    outJson["EVReady"] = DC_EVStatusTypeDoc.EVReady;
    if (DC_EVStatusTypeDoc.EVCabinConditioning_isUsed) {
        outJson["EVCabinConditioning"] = DC_EVStatusTypeDoc.EVCabinConditioning;
    }
    if (DC_EVStatusTypeDoc.EVRESSConditioning_isUsed) {
        outJson["EVRESSConditioning"] = DC_EVStatusTypeDoc.EVRESSConditioning;
    }
    outJson["EVErrorCode"] = DC_EVStatusTypeDoc.EVErrorCode;
    outJson["EVRESSSOC"] = DC_EVStatusTypeDoc.EVRESSSOC;

    return outJson;
}

struct din_DC_EVStatusType getDoc_DC_EVStatusType(const json& DC_EVStatusTypeJson) {
    struct din_DC_EVStatusType outDoc;
    init_din_DC_EVStatusType(&outDoc);

    outDoc.EVReady = DC_EVStatusTypeJson["EVReady"].template get<int>();
    if (DC_EVStatusTypeJson.contains("EVCabinConditioning")) {
        outDoc.EVCabinConditioning_isUsed = 1;
        outDoc.EVCabinConditioning = DC_EVStatusTypeJson["EVCabinConditioning"].template get<int>();
    } else {
        outDoc.EVCabinConditioning_isUsed = 0;
    }
    if (DC_EVStatusTypeJson.contains("EVRESSConditioning")) {
        outDoc.EVRESSConditioning_isUsed = 1;
        outDoc.EVRESSConditioning = DC_EVStatusTypeJson["EVRESSConditioning"].template get<int>();
    } else {
        outDoc.EVRESSConditioning_isUsed = 0;
    }
    outDoc.EVErrorCode = DC_EVStatusTypeJson["EVErrorCode"].template get<din_DC_EVErrorCodeType>();
    outDoc.EVRESSSOC = DC_EVStatusTypeJson["EVRESSSOC"].template get<int8_t>();

    return outDoc;
}

json getJson_SignatureValueType(const struct din_SignatureValueType& SignatureValueTypeDoc) {
    json outJson;

    if (SignatureValueTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = SignatureValueTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < SignatureValueTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = SignatureValueTypeDoc.Id.characters[i];
        }
    }
    outJson["CONTENT"]["bytesLen"] = SignatureValueTypeDoc.CONTENT.bytesLen;
    for (uint16_t i = 0; i < SignatureValueTypeDoc.CONTENT.bytesLen; i++) {
        outJson["CONTENT"]["bytes"][i] = SignatureValueTypeDoc.CONTENT.bytes[i];
    }

    return outJson;
}

struct din_SignatureValueType getDoc_SignatureValueType(const json& SignatureValueTypeJson) {
    struct din_SignatureValueType outDoc;
    init_din_SignatureValueType(&outDoc);

    if (SignatureValueTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = SignatureValueTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = SignatureValueTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    outDoc.CONTENT.bytesLen = SignatureValueTypeJson["CONTENT"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.CONTENT.bytesLen; i++) {
        outDoc.CONTENT.bytes[i] = SignatureValueTypeJson["CONTENT"]["bytes"][i].template get<uint8_t>();
    }

    return outDoc;
}

json getJson_CertificateChainType(const struct din_CertificateChainType& CertificateChainTypeDoc) {
    json outJson;

    outJson["Certificate"]["bytesLen"] = CertificateChainTypeDoc.Certificate.bytesLen;
    for (uint16_t i = 0; i < CertificateChainTypeDoc.Certificate.bytesLen; i++) {
        outJson["Certificate"]["bytes"][i] = CertificateChainTypeDoc.Certificate.bytes[i];
    }
    if (CertificateChainTypeDoc.SubCertificates_isUsed) {
        outJson["SubCertificates"] = getJson_SubCertificatesType(CertificateChainTypeDoc.SubCertificates);
    }

    return outJson;
}

struct din_CertificateChainType getDoc_CertificateChainType(const json& CertificateChainTypeJson) {
    struct din_CertificateChainType outDoc;
    init_din_CertificateChainType(&outDoc);

    outDoc.Certificate.bytesLen = CertificateChainTypeJson["Certificate"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Certificate.bytesLen; i++) {
        outDoc.Certificate.bytes[i] = CertificateChainTypeJson["Certificate"]["bytes"][i].template get<uint8_t>();
    }
    if (CertificateChainTypeJson.contains("SubCertificates")) {
        outDoc.SubCertificates_isUsed = 1;
        outDoc.SubCertificates = getDoc_SubCertificatesType(CertificateChainTypeJson["SubCertificates"]);
    } else {
        outDoc.SubCertificates_isUsed = 0;
    }

    return outDoc;
}

json getJson_DC_EVSEStatusType(const struct din_DC_EVSEStatusType& DC_EVSEStatusTypeDoc) {
    json outJson;

    if (DC_EVSEStatusTypeDoc.EVSEIsolationStatus_isUsed) {
        outJson["EVSEIsolationStatus"] = DC_EVSEStatusTypeDoc.EVSEIsolationStatus;
    }
    outJson["EVSEStatusCode"] = DC_EVSEStatusTypeDoc.EVSEStatusCode;
    outJson["NotificationMaxDelay"] = DC_EVSEStatusTypeDoc.NotificationMaxDelay;
    outJson["EVSENotification"] = DC_EVSEStatusTypeDoc.EVSENotification;

    return outJson;
}

struct din_DC_EVSEStatusType getDoc_DC_EVSEStatusType(const json& DC_EVSEStatusTypeJson) {
    struct din_DC_EVSEStatusType outDoc;
    init_din_DC_EVSEStatusType(&outDoc);

    if (DC_EVSEStatusTypeJson.contains("EVSEIsolationStatus")) {
        outDoc.EVSEIsolationStatus_isUsed = 1;
        outDoc.EVSEIsolationStatus = DC_EVSEStatusTypeJson["EVSEIsolationStatus"].template get<din_isolationLevelType>();
    } else {
        outDoc.EVSEIsolationStatus_isUsed = 0;
    }
    outDoc.EVSEStatusCode = DC_EVSEStatusTypeJson["EVSEStatusCode"].template get<din_DC_EVSEStatusCodeType>();
    outDoc.NotificationMaxDelay = DC_EVSEStatusTypeJson["NotificationMaxDelay"].template get<uint16_t>();
    outDoc.EVSENotification = DC_EVSEStatusTypeJson["EVSENotification"].template get<din_EVSENotificationType>();

    return outDoc;
}

json getJson_PhysicalValueType(const struct din_PhysicalValueType& PhysicalValueTypeDoc) {
    json outJson;

    outJson["Multiplier"] = PhysicalValueTypeDoc.Multiplier;
    if (PhysicalValueTypeDoc.Unit_isUsed) {
        outJson["Unit"] = PhysicalValueTypeDoc.Unit;
    }
    outJson["Value"] = PhysicalValueTypeDoc.Value;

    return outJson;
}

struct din_PhysicalValueType getDoc_PhysicalValueType(const json& PhysicalValueTypeJson) {
    struct din_PhysicalValueType outDoc;
    init_din_PhysicalValueType(&outDoc);

    outDoc.Multiplier = PhysicalValueTypeJson["Multiplier"].template get<int8_t>();
    if (PhysicalValueTypeJson.contains("Unit")) {
        outDoc.Unit_isUsed = 1;
        outDoc.Unit = PhysicalValueTypeJson["Unit"].template get<din_unitSymbolType>();
    } else {
        outDoc.Unit_isUsed = 0;
    }
    outDoc.Value = PhysicalValueTypeJson["Value"].template get<int32_t>();

    return outDoc;
}

json getJson_ParameterType(const struct din_ParameterType& ParameterTypeDoc) {
    json outJson;

    outJson["Name"]["charactersLen"] = ParameterTypeDoc.Name.charactersLen;
    for (uint16_t i = 0; i < ParameterTypeDoc.Name.charactersLen; i++) {
        outJson["Name"]["characters"][i] = ParameterTypeDoc.Name.characters[i];
    }
    outJson["ValueType"] = ParameterTypeDoc.ValueType;
    if (ParameterTypeDoc.boolValue_isUsed) {
        outJson["boolValue"] = ParameterTypeDoc.boolValue;
    }
    if (ParameterTypeDoc.byteValue_isUsed) {
        outJson["byteValue"] = ParameterTypeDoc.byteValue;
    }
    if (ParameterTypeDoc.shortValue_isUsed) {
        outJson["shortValue"] = ParameterTypeDoc.shortValue;
    }
    if (ParameterTypeDoc.intValue_isUsed) {
        outJson["intValue"] = ParameterTypeDoc.intValue;
    }
    if (ParameterTypeDoc.physicalValue_isUsed) {
        outJson["physicalValue"] = getJson_PhysicalValueType(ParameterTypeDoc.physicalValue);
    }
    if (ParameterTypeDoc.stringValue_isUsed) {
        outJson["stringValue"]["charactersLen"] = ParameterTypeDoc.stringValue.charactersLen;
        for (uint16_t i = 0; i < ParameterTypeDoc.stringValue.charactersLen; i++) {
            outJson["stringValue"]["characters"][i] = ParameterTypeDoc.stringValue.characters[i];
        }
    }

    return outJson;
}

struct din_ParameterType getDoc_ParameterType(const json& ParameterTypeJson) {
    struct din_ParameterType outDoc;
    init_din_ParameterType(&outDoc);

    outDoc.Name.charactersLen = ParameterTypeJson["Name"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Name.charactersLen; i++) {
        outDoc.Name.characters[i] = ParameterTypeJson["Name"]["characters"][i].template get<char>();
    }
    outDoc.ValueType = ParameterTypeJson["ValueType"].template get<din_valueType>();
    if (ParameterTypeJson.contains("boolValue")) {
        outDoc.boolValue_isUsed = 1;
        outDoc.boolValue = ParameterTypeJson["boolValue"].template get<int>();
    } else {
        outDoc.boolValue_isUsed = 0;
    }
    if (ParameterTypeJson.contains("byteValue")) {
        outDoc.byteValue_isUsed = 1;
        outDoc.byteValue = ParameterTypeJson["byteValue"].template get<uint8_t>();
    } else {
        outDoc.byteValue_isUsed = 0;
    }
    if (ParameterTypeJson.contains("shortValue")) {
        outDoc.shortValue_isUsed = 1;
        outDoc.shortValue = ParameterTypeJson["shortValue"].template get<int16_t>();
    } else {
        outDoc.shortValue_isUsed = 0;
    }
    if (ParameterTypeJson.contains("intValue")) {
        outDoc.intValue_isUsed = 1;
        outDoc.intValue = ParameterTypeJson["intValue"].template get<int32_t>();
    } else {
        outDoc.intValue_isUsed = 0;
    }
    if (ParameterTypeJson.contains("physicalValue")) {
        outDoc.physicalValue_isUsed = 1;
        outDoc.physicalValue = getDoc_PhysicalValueType(ParameterTypeJson["physicalValue"]);
    } else {
        outDoc.physicalValue_isUsed = 0;
    }
    if (ParameterTypeJson.contains("stringValue")) {
        outDoc.stringValue_isUsed = 1;
        outDoc.stringValue.charactersLen = ParameterTypeJson["stringValue"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.stringValue.charactersLen; i++) {
            outDoc.stringValue.characters[i] = ParameterTypeJson["stringValue"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.stringValue_isUsed = 0;
    }

    return outDoc;
}

json getJson_ParameterSetType(const struct din_ParameterSetType& ParameterSetTypeDoc) {
    json outJson;

    outJson["ParameterSetID"] = ParameterSetTypeDoc.ParameterSetID;
    outJson["Parameter"] = getJson_ParameterType(ParameterSetTypeDoc.Parameter);

    return outJson;
}

struct din_ParameterSetType getDoc_ParameterSetType(const json& ParameterSetTypeJson) {
    struct din_ParameterSetType outDoc;
    init_din_ParameterSetType(&outDoc);

    outDoc.ParameterSetID = ParameterSetTypeJson["ParameterSetID"].template get<uint8_t>();
    outDoc.Parameter = getDoc_ParameterType(ParameterSetTypeJson["Parameter"]);

    return outDoc;
}

json getJson_ListOfRootCertificateIDsType(const struct din_ListOfRootCertificateIDsType& ListOfRootCertificateIDsTypeDoc) {
    json outJson;

    outJson["RootCertificateID"]["arrayLen"] = ListOfRootCertificateIDsTypeDoc.RootCertificateID.arrayLen;
    for (uint16_t i = 0; i < ListOfRootCertificateIDsTypeDoc.RootCertificateID.arrayLen; i++) {
        outJson["RootCertificateID"]["array"][i]["charactersLen"] = ListOfRootCertificateIDsTypeDoc.RootCertificateID.array[i].charactersLen;
        for (uint16_t j = 0; j < ListOfRootCertificateIDsTypeDoc.RootCertificateID.array[i].charactersLen; j++) {
            outJson["RootCertificateID"]["array"][i]["characters"][j] = ListOfRootCertificateIDsTypeDoc.RootCertificateID.array[i].characters[j];
        }
    }

    return outJson;
}

struct din_ListOfRootCertificateIDsType getDoc_ListOfRootCertificateIDsType(const json& ListOfRootCertificateIDsTypeJson) {
    struct din_ListOfRootCertificateIDsType outDoc;
    init_din_ListOfRootCertificateIDsType(&outDoc);

    outDoc.RootCertificateID.arrayLen = ListOfRootCertificateIDsTypeJson["RootCertificateID"]["arrayLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.RootCertificateID.arrayLen; i++) {
        outDoc.RootCertificateID.array[i].charactersLen = ListOfRootCertificateIDsTypeJson["RootCertificateID"]["array"][i]["charactersLen"].template get<uint16_t>();
        for (uint16_t j = 0; j < outDoc.RootCertificateID.array[i].charactersLen; j++) {
            outDoc.RootCertificateID.array[i].characters[j] = ListOfRootCertificateIDsTypeJson["RootCertificateID"]["array"][i]["characters"][j].template get<char>();
        }
    }

    return outDoc;
}

json getJson_PaymentOptionsType(const struct din_PaymentOptionsType& PaymentOptionsTypeDoc) {
    json outJson;

    outJson["PaymentOption"]["arrayLen"] = PaymentOptionsTypeDoc.PaymentOption.arrayLen;
    for (uint16_t i = 0; i < PaymentOptionsTypeDoc.PaymentOption.arrayLen; i++) {
        outJson["PaymentOption"]["array"][i] = PaymentOptionsTypeDoc.PaymentOption.array[i];
    }

    return outJson;
}

struct din_PaymentOptionsType getDoc_PaymentOptionsType(const json& PaymentOptionsTypeJson) {
    struct din_PaymentOptionsType outDoc;
    init_din_PaymentOptionsType(&outDoc);

    outDoc.PaymentOption.arrayLen = PaymentOptionsTypeJson["PaymentOption"]["arrayLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.PaymentOption.arrayLen; i++) {
        outDoc.PaymentOption.array[i] = PaymentOptionsTypeJson["PaymentOption"]["array"][i].template get<din_paymentOptionType>();
    }

    return outDoc;
}

json getJson_SelectedServiceListType(const struct din_SelectedServiceListType& SelectedServiceListTypeDoc) {
    json outJson;

    outJson["SelectedService"]["arrayLen"] = SelectedServiceListTypeDoc.SelectedService.arrayLen;
    for (uint16_t i = 0; i < SelectedServiceListTypeDoc.SelectedService.arrayLen; i++) {
        outJson["SelectedService"]["array"][i] = getJson_SelectedServiceType(SelectedServiceListTypeDoc.SelectedService.array[i]);
    }

    return outJson;
}

struct din_SelectedServiceListType getDoc_SelectedServiceListType(const json& SelectedServiceListTypeJson) {
    struct din_SelectedServiceListType outDoc;
    init_din_SelectedServiceListType(&outDoc);

    outDoc.SelectedService.arrayLen = SelectedServiceListTypeJson["SelectedService"]["arrayLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.SelectedService.arrayLen; i++) {
        outDoc.SelectedService.array[i] = getDoc_SelectedServiceType(SelectedServiceListTypeJson["SelectedService"]["array"][i]);
    }

    return outDoc;
}

json getJson_AC_EVChargeParameterType(const struct din_AC_EVChargeParameterType& AC_EVChargeParameterTypeDoc) {
    json outJson;

    outJson["DepartureTime"] = AC_EVChargeParameterTypeDoc.DepartureTime;
    outJson["EAmount"] = getJson_PhysicalValueType(AC_EVChargeParameterTypeDoc.EAmount);
    outJson["EVMaxVoltage"] = getJson_PhysicalValueType(AC_EVChargeParameterTypeDoc.EVMaxVoltage);
    outJson["EVMaxCurrent"] = getJson_PhysicalValueType(AC_EVChargeParameterTypeDoc.EVMaxCurrent);
    outJson["EVMinCurrent"] = getJson_PhysicalValueType(AC_EVChargeParameterTypeDoc.EVMinCurrent);
    
    return outJson;
}

struct din_AC_EVChargeParameterType getDoc_AC_EVChargeParameterType(const json& AC_EVChargeParameterTypeJson) {
    struct din_AC_EVChargeParameterType outDoc;
    init_din_AC_EVChargeParameterType(&outDoc);

    outDoc.DepartureTime = AC_EVChargeParameterTypeJson["DepartureTime"].template get<uint32_t>();
    outDoc.EAmount = getDoc_PhysicalValueType(AC_EVChargeParameterTypeJson["EAmount"]);
    outDoc.EVMaxVoltage = getDoc_PhysicalValueType(AC_EVChargeParameterTypeJson["EVMaxVoltage"]);
    outDoc.EVMaxCurrent = getDoc_PhysicalValueType(AC_EVChargeParameterTypeJson["EVMaxCurrent"]);
    outDoc.EVMinCurrent = getDoc_PhysicalValueType(AC_EVChargeParameterTypeJson["EVMinCurrent"]);
    
    return outDoc;
}

json getJson_DC_EVChargeParameterType(const struct din_DC_EVChargeParameterType& DC_EVChargeParameterTypeDoc) {
    json outJson;

    outJson["DC_EVStatus"] = getJson_DC_EVStatusType(DC_EVChargeParameterTypeDoc.DC_EVStatus);
    outJson["EVMaximumCurrentLimit"] = getJson_PhysicalValueType(DC_EVChargeParameterTypeDoc.EVMaximumCurrentLimit);
    if (DC_EVChargeParameterTypeDoc.EVMaximumPowerLimit_isUsed) {
        outJson["EVMaximumPowerLimit"] = getJson_PhysicalValueType(DC_EVChargeParameterTypeDoc.EVMaximumPowerLimit);
    }
    outJson["EVMaximumVoltageLimit"] = getJson_PhysicalValueType(DC_EVChargeParameterTypeDoc.EVMaximumVoltageLimit);
    if (DC_EVChargeParameterTypeDoc.EVEnergyCapacity_isUsed) {
        outJson["EVEnergyCapacity"] = getJson_PhysicalValueType(DC_EVChargeParameterTypeDoc.EVEnergyCapacity);
    }
    if (DC_EVChargeParameterTypeDoc.EVEnergyRequest_isUsed) {
        outJson["EVEnergyRequest"] = getJson_PhysicalValueType(DC_EVChargeParameterTypeDoc.EVEnergyRequest);
    }
    if (DC_EVChargeParameterTypeDoc.FullSOC_isUsed) {
        outJson["FullSOC"] = DC_EVChargeParameterTypeDoc.FullSOC;
    }
    if (DC_EVChargeParameterTypeDoc.BulkSOC_isUsed) {
        outJson["BulkSOC"] = DC_EVChargeParameterTypeDoc.BulkSOC;
    }

    return outJson;
}

struct din_DC_EVChargeParameterType getDoc_DC_EVChargeParameterType(const json& DC_EVChargeParameterTypeJson) {
    struct din_DC_EVChargeParameterType outDoc;
    init_din_DC_EVChargeParameterType(&outDoc);

    outDoc.DC_EVStatus = getDoc_DC_EVStatusType(DC_EVChargeParameterTypeJson["DC_EVStatus"]);
    outDoc.EVMaximumCurrentLimit = getDoc_PhysicalValueType(DC_EVChargeParameterTypeJson["EVMaximumCurrentLimit"]);
    if (DC_EVChargeParameterTypeJson.contains("EVMaximumPowerLimit")) {
        outDoc.EVMaximumPowerLimit_isUsed = 1;
        outDoc.EVMaximumPowerLimit = getDoc_PhysicalValueType(DC_EVChargeParameterTypeJson["EVMaximumPowerLimit"]);
    } else {
        outDoc.EVMaximumPowerLimit_isUsed = 0;
    }
    outDoc.EVMaximumVoltageLimit = getDoc_PhysicalValueType(DC_EVChargeParameterTypeJson["EVMaximumVoltageLimit"]);
    if (DC_EVChargeParameterTypeJson.contains("EVEnergyCapacity")) {
        outDoc.EVEnergyCapacity_isUsed = 1;
        outDoc.EVEnergyCapacity = getDoc_PhysicalValueType(DC_EVChargeParameterTypeJson["EVEnergyCapacity"]);
    } else {
        outDoc.EVEnergyCapacity_isUsed = 0;
    }
    if (DC_EVChargeParameterTypeJson.contains("EVEnergyRequest")) {
        outDoc.EVEnergyRequest_isUsed = 1;
        outDoc.EVEnergyRequest = getDoc_PhysicalValueType(DC_EVChargeParameterTypeJson["EVEnergyRequest"]);
    } else {
        outDoc.EVEnergyRequest_isUsed = 0;
    }
    if (DC_EVChargeParameterTypeJson.contains("FullSOC")) {
        outDoc.FullSOC_isUsed = 1;
        outDoc.FullSOC = DC_EVChargeParameterTypeJson["FullSOC"].template get<int8_t>();
    } else {
        outDoc.FullSOC_isUsed = 0;
    }
    if (DC_EVChargeParameterTypeJson.contains("BulkSOC")) {
        outDoc.BulkSOC_isUsed = 1;
        outDoc.BulkSOC = DC_EVChargeParameterTypeJson["BulkSOC"].template get<int8_t>();
    } else {
        outDoc.BulkSOC_isUsed = 0;
    }

    return outDoc;
}

json getJson_EVChargeParameterType(const struct din_EVChargeParameterType& EVChargeParameterTypeDoc) {
    json outJson;

    outJson["_unused"] = EVChargeParameterTypeDoc._unused;
    
    return outJson;
}

struct din_EVChargeParameterType getDoc_EVChargeParameterType(const json& EVChargeParameterTypeJson) {
    struct din_EVChargeParameterType outDoc;
    init_din_EVChargeParameterType(&outDoc);

    outDoc._unused = EVChargeParameterTypeJson["_unused"].template get<int>();
    
    return outDoc;
}

json getJson_ChargingProfileType(const struct din_ChargingProfileType& ChargingProfileTypeDoc) {
    json outJson;

    outJson["SAScheduleTupleID"] = ChargingProfileTypeDoc.SAScheduleTupleID;
    outJson["ProfileEntry"]["arrayLen"] = ChargingProfileTypeDoc.ProfileEntry.arrayLen;
    for (uint16_t i = 0; i < ChargingProfileTypeDoc.ProfileEntry.arrayLen; i++) {
        outJson["ProfileEntry"]["array"][i] = getJson_ProfileEntryType(ChargingProfileTypeDoc.ProfileEntry.array[i]);
    }

    return outJson;
}

struct din_ChargingProfileType getDoc_ChargingProfileType(const json& ChargingProfileTypeJson) {
    struct din_ChargingProfileType outDoc;
    init_din_ChargingProfileType(&outDoc);

    outDoc.SAScheduleTupleID = ChargingProfileTypeJson["SAScheduleTupleID"].template get<uint8_t>();
    outDoc.ProfileEntry.arrayLen = ChargingProfileTypeJson["ProfileEntry"]["arrayLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.ProfileEntry.arrayLen; i++) {
        outDoc.ProfileEntry.array[i] = getDoc_ProfileEntryType(ChargingProfileTypeJson["ProfileEntry"]["array"][i]);
    }

    return outDoc;
}

json getJson_EVSEStatusType(const struct din_EVSEStatusType& EVSEStatusTypeDoc) {
    json outJson;

    outJson["_unused"] = EVSEStatusTypeDoc._unused;
    
    return outJson;
}

struct din_EVSEStatusType getDoc_EVSEStatusType(const json& EVSEStatusTypeJson) {
    struct din_EVSEStatusType outDoc;
    init_din_EVSEStatusType(&outDoc);

    outDoc._unused = EVSEStatusTypeJson["_unused"].template get<int>();
    
    return outDoc;
}

json getJson_KeyInfoType(const struct din_KeyInfoType& KeyInfoTypeDoc) {
    json outJson;

    if (KeyInfoTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = KeyInfoTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < KeyInfoTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = KeyInfoTypeDoc.Id.characters[i];
        }
    }
    if (KeyInfoTypeDoc.KeyName_isUsed) {
        outJson["KeyName"]["charactersLen"] = KeyInfoTypeDoc.KeyName.charactersLen;
        for (uint16_t i = 0; i < KeyInfoTypeDoc.KeyName.charactersLen; i++) {
            outJson["KeyName"]["characters"][i] = KeyInfoTypeDoc.KeyName.characters[i];
        }
    }
    if (KeyInfoTypeDoc.KeyValue_isUsed) {
        outJson["KeyValue"] = getJson_KeyValueType(KeyInfoTypeDoc.KeyValue);
    }
    if (KeyInfoTypeDoc.RetrievalMethod_isUsed) {
        outJson["RetrievalMethod"] = getJson_RetrievalMethodType(KeyInfoTypeDoc.RetrievalMethod);
    }
    if (KeyInfoTypeDoc.X509Data_isUsed) {
        outJson["X509Data"] = getJson_X509DataType(KeyInfoTypeDoc.X509Data);
    }
    if (KeyInfoTypeDoc.PGPData_isUsed) {
        outJson["PGPData"] = getJson_PGPDataType(KeyInfoTypeDoc.PGPData);
    }
    if (KeyInfoTypeDoc.SPKIData_isUsed) {
        outJson["SPKIData"] = getJson_SPKIDataType(KeyInfoTypeDoc.SPKIData);
    }
    if (KeyInfoTypeDoc.MgmtData_isUsed) {
        outJson["MgmtData"]["charactersLen"] = KeyInfoTypeDoc.MgmtData.charactersLen;
        for (uint16_t i = 0; i < KeyInfoTypeDoc.MgmtData.charactersLen; i++) {
            outJson["MgmtData"]["characters"][i] = KeyInfoTypeDoc.MgmtData.characters[i];
        }
    }
    if (KeyInfoTypeDoc.ANY_isUsed) {
        outJson["ANY"]["bytesLen"] = KeyInfoTypeDoc.ANY.bytesLen;
        for (uint16_t i = 0; i < KeyInfoTypeDoc.ANY.bytesLen; i++) {
            outJson["ANY"]["bytes"][i] = KeyInfoTypeDoc.ANY.bytes[i];
        }
    }

    return outJson;
}

struct din_KeyInfoType getDoc_KeyInfoType(const json& KeyInfoTypeJson) {
    struct din_KeyInfoType outDoc;
    init_din_KeyInfoType(&outDoc);

    if (KeyInfoTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = KeyInfoTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = KeyInfoTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    if (KeyInfoTypeJson.contains("KeyName")) {
        outDoc.KeyName_isUsed = 1;
        outDoc.KeyName.charactersLen = KeyInfoTypeJson["KeyName"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.KeyName.charactersLen; i++) {
            outDoc.KeyName.characters[i] = KeyInfoTypeJson["KeyName"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.KeyName_isUsed = 0;
    }
    if (KeyInfoTypeJson.contains("KeyValue")) {
        outDoc.KeyValue_isUsed = 1;
        outDoc.KeyValue = getDoc_KeyValueType(KeyInfoTypeJson["KeyValue"]);
    } else {
        outDoc.KeyValue_isUsed = 0;
    }
    if (KeyInfoTypeJson.contains("RetrievalMethod")) {
        outDoc.RetrievalMethod_isUsed = 1;
        outDoc.RetrievalMethod = getDoc_RetrievalMethodType(KeyInfoTypeJson["RetrievalMethod"]);
    } else {
        outDoc.RetrievalMethod_isUsed = 0;
    }
    if (KeyInfoTypeJson.contains("X509Data")) {
        outDoc.X509Data_isUsed = 1;
        outDoc.X509Data = getDoc_X509DataType(KeyInfoTypeJson["X509Data"]);
    } else {
        outDoc.X509Data_isUsed = 0;
    }
    if (KeyInfoTypeJson.contains("PGPData")) {
        outDoc.PGPData_isUsed = 1;
        outDoc.PGPData = getDoc_PGPDataType(KeyInfoTypeJson["PGPData"]);
    } else {
        outDoc.PGPData_isUsed = 0;
    }
    if (KeyInfoTypeJson.contains("SPKIData")) {
        outDoc.SPKIData_isUsed = 1;
        outDoc.SPKIData = getDoc_SPKIDataType(KeyInfoTypeJson["SPKIData"]);
    } else {
        outDoc.SPKIData_isUsed = 0;
    }
    if (KeyInfoTypeJson.contains("MgmtData")) {
        outDoc.MgmtData_isUsed = 1;
        outDoc.MgmtData.charactersLen = KeyInfoTypeJson["MgmtData"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.MgmtData.charactersLen; i++) {
            outDoc.MgmtData.characters[i] = KeyInfoTypeJson["MgmtData"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.MgmtData_isUsed = 0;
    }
    if (KeyInfoTypeJson.contains("ANY")) {
        outDoc.ANY_isUsed = 1;
        outDoc.ANY.bytesLen = KeyInfoTypeJson["ANY"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ANY.bytesLen; i++) {
            outDoc.ANY.bytes[i] = KeyInfoTypeJson["ANY"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.ANY_isUsed = 0;
    }

    return outDoc;
}

json getJson_ServiceChargeType(const struct din_ServiceChargeType& ServiceChargeTypeDoc) {
    json outJson;

    outJson["ServiceTag"] = getJson_ServiceTagType(ServiceChargeTypeDoc.ServiceTag);
    outJson["FreeService"] = ServiceChargeTypeDoc.FreeService;
    outJson["EnergyTransferType"] = ServiceChargeTypeDoc.EnergyTransferType;

    return outJson;
}

struct din_ServiceChargeType getDoc_ServiceChargeType(const json& ServiceChargeTypeJson) {
    struct din_ServiceChargeType outDoc;
    init_din_ServiceChargeType(&outDoc);

    outDoc.ServiceTag = getDoc_ServiceTagType(ServiceChargeTypeJson["ServiceTag"]);
    outDoc.FreeService = ServiceChargeTypeJson["FreeService"].template get<int>();
    outDoc.EnergyTransferType = ServiceChargeTypeJson["EnergyTransferType"].template get<din_EVSESupportedEnergyTransferType>();

    return outDoc;
}

json getJson_ServiceParameterListType(const struct din_ServiceParameterListType& ServiceParameterListTypeDoc) {
    json outJson;

    outJson["ParameterSet"]["arrayLen"] = ServiceParameterListTypeDoc.ParameterSet.arrayLen;
    for (uint16_t i = 0; i < ServiceParameterListTypeDoc.ParameterSet.arrayLen; i++) {
        outJson["ParameterSet"]["array"][i] = getJson_ParameterSetType(ServiceParameterListTypeDoc.ParameterSet.array[i]);
    }

    return outJson;
}

struct din_ServiceParameterListType getDoc_ServiceParameterListType(const json& ServiceParameterListTypeJson) {
    struct din_ServiceParameterListType outDoc;
    init_din_ServiceParameterListType(&outDoc);

    outDoc.ParameterSet.arrayLen = ServiceParameterListTypeJson["ParameterSet"]["arrayLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.ParameterSet.arrayLen; i++) {
        outDoc.ParameterSet.array[i] = getDoc_ParameterSetType(ServiceParameterListTypeJson["ParameterSet"]["array"][i]);
    }

    return outDoc;
}

json getJson_SAScheduleListType(const struct din_SAScheduleListType& SAScheduleListTypeDoc) {
    json outJson;

    outJson["SAScheduleTuple"]["arrayLen"] = SAScheduleListTypeDoc.SAScheduleTuple.arrayLen;
    for (uint16_t i = 0; i < SAScheduleListTypeDoc.SAScheduleTuple.arrayLen; i++) {
        outJson["SAScheduleTuple"]["array"][i] = getJson_SAScheduleTupleType(SAScheduleListTypeDoc.SAScheduleTuple.array[i]);
    }

    return outJson;
}

struct din_SAScheduleListType getDoc_SAScheduleListType(const json& SAScheduleListTypeJson) {
    struct din_SAScheduleListType outDoc;
    init_din_SAScheduleListType(&outDoc);

    outDoc.SAScheduleTuple.arrayLen = SAScheduleListTypeJson["SAScheduleTuple"]["arrayLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.SAScheduleTuple.arrayLen; i++) {
        outDoc.SAScheduleTuple.array[i] = getDoc_SAScheduleTupleType(SAScheduleListTypeJson["SAScheduleTuple"]["array"][i]);
    }

    return outDoc;
}

json getJson_SASchedulesType(const struct din_SASchedulesType& SASchedulesTypeDoc) {
    json outJson;

    outJson["_unused"] = SASchedulesTypeDoc._unused;

    return outJson;
}

struct din_SASchedulesType getDoc_SASchedulesType(const json& SASchedulesTypeJson) {
    struct din_SASchedulesType outDoc;
    init_din_SASchedulesType(&outDoc);

    outDoc._unused = SASchedulesTypeJson["_unused"].template get<int>();

    return outDoc;
}

json getJson_DC_EVPowerDeliveryParameterType(const struct din_DC_EVPowerDeliveryParameterType& DC_EVPowerDeliveryParameterTypeDoc) {
    json outJson;

    outJson["DC_EVStatus"] = getJson_DC_EVStatusType(DC_EVPowerDeliveryParameterTypeDoc.DC_EVStatus);
    if (DC_EVPowerDeliveryParameterTypeDoc.BulkChargingComplete_isUsed) {
        outJson["BulkChargingComplete"] = DC_EVPowerDeliveryParameterTypeDoc.BulkChargingComplete;
    }
    outJson["ChargingComplete"] = DC_EVPowerDeliveryParameterTypeDoc.ChargingComplete;

    return outJson;
}

struct din_DC_EVPowerDeliveryParameterType getDoc_DC_EVPowerDeliveryParameterType(const json& DC_EVPowerDeliveryParameterTypeJson) {
    struct din_DC_EVPowerDeliveryParameterType outDoc;
    init_din_DC_EVPowerDeliveryParameterType(&outDoc);

    outDoc.DC_EVStatus = getDoc_DC_EVStatusType(DC_EVPowerDeliveryParameterTypeJson["DC_EVStatus"]);
    if (DC_EVPowerDeliveryParameterTypeJson.contains("BulkChargingComplete")) {
        outDoc.BulkChargingComplete_isUsed = 1;
        outDoc.BulkChargingComplete = DC_EVPowerDeliveryParameterTypeJson["BulkChargingComplete"].template get<int>();
    } else {
        outDoc.BulkChargingComplete_isUsed = 0;
    }
    outDoc.ChargingComplete = DC_EVPowerDeliveryParameterTypeJson["ChargingComplete"].template get<int>();

    return outDoc;
}

json getJson_EVPowerDeliveryParameterType(const struct din_EVPowerDeliveryParameterType& EVPowerDeliveryParameterTypeDoc) {
    json outJson;

    outJson["_unused"] = EVPowerDeliveryParameterTypeDoc._unused;

    return outJson;
}

struct din_EVPowerDeliveryParameterType getDoc_EVPowerDeliveryParameterType(const json& EVPowerDeliveryParameterTypeJson) {
    struct din_EVPowerDeliveryParameterType outDoc;
    init_din_EVPowerDeliveryParameterType(&outDoc);

    outDoc._unused = EVPowerDeliveryParameterTypeJson["_unused"].template get<int>();

    return outDoc;
}

json getJson_ObjectType(const struct din_ObjectType& ObjectTypeDoc) {
    json outJson;

    if (ObjectTypeDoc.Encoding_isUsed) {
        outJson["Encoding"]["charactersLen"] = ObjectTypeDoc.Encoding.charactersLen;
        for (uint16_t i = 0; i < ObjectTypeDoc.Encoding.charactersLen; i++) {
            outJson["Encoding"]["characters"][i] = ObjectTypeDoc.Encoding.characters[i];
        }
    }
    if (ObjectTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = ObjectTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < ObjectTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = ObjectTypeDoc.Id.characters[i];
        }
    }
    if (ObjectTypeDoc.MimeType_isUsed) {
        outJson["MimeType"]["charactersLen"] = ObjectTypeDoc.MimeType.charactersLen;
        for (uint16_t i = 0; i < ObjectTypeDoc.MimeType.charactersLen; i++) {
            outJson["MimeType"]["characters"][i] = ObjectTypeDoc.MimeType.characters[i];
        }
    }
    if (ObjectTypeDoc.ANY_isUsed) {
        outJson["ANY"]["bytesLen"] = ObjectTypeDoc.ANY.bytesLen;
        for (uint16_t i = 0; i < ObjectTypeDoc.ANY.bytesLen; i++) {
            outJson["ANY"]["bytes"][i] = ObjectTypeDoc.ANY.bytes[i];
        }
    }

    return outJson;
}

struct din_ObjectType getDoc_ObjectType(const json& ObjectTypeJson) {
    struct din_ObjectType outDoc;
    init_din_ObjectType(&outDoc);

    if (ObjectTypeJson.contains("Encoding")) {
        outDoc.Encoding_isUsed = 1;
        outDoc.Encoding.charactersLen = ObjectTypeJson["Encoding"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Encoding.charactersLen; i++) {
            outDoc.Encoding.characters[i] = ObjectTypeJson["Encoding"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Encoding_isUsed = 0;
    }
    if (ObjectTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = ObjectTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = ObjectTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    if (ObjectTypeJson.contains("MimeType")) {
        outDoc.MimeType_isUsed = 1;
        outDoc.MimeType.charactersLen = ObjectTypeJson["MimeType"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.MimeType.charactersLen; i++) {
            outDoc.MimeType.characters[i] = ObjectTypeJson["MimeType"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.MimeType_isUsed = 0;
    }
    if (ObjectTypeJson.contains("ANY")) {
        outDoc.ANY_isUsed = 1;
        outDoc.ANY.bytesLen = ObjectTypeJson["ANY"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ANY.bytesLen; i++) {
            outDoc.ANY.bytes[i] = ObjectTypeJson["ANY"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.ANY_isUsed = 0;
    }

    return outDoc;
}

json getJson_ServiceTagListType(const struct din_ServiceTagListType& ServiceTagListTypeDoc) {
    json outJson;

    outJson["Service"] = getJson_ServiceType(ServiceTagListTypeDoc.Service);

    return outJson;
}

struct din_ServiceTagListType getDoc_ServiceTagListType(const json& ServiceTagListTypeJson) {
    struct din_ServiceTagListType outDoc;
    init_din_ServiceTagListType(&outDoc);

    outDoc.Service = getDoc_ServiceType(ServiceTagListTypeJson["Service"]);

    return outDoc;
}

json getJson_DC_EVSEChargeParameterType(const struct din_DC_EVSEChargeParameterType& DC_EVSEChargeParameterTypeDoc) {
    json outJson;

    outJson["DC_EVSEStatus"] = getJson_DC_EVSEStatusType(DC_EVSEChargeParameterTypeDoc.DC_EVSEStatus);
    outJson["EVSEMaximumCurrentLimit"] = getJson_PhysicalValueType(DC_EVSEChargeParameterTypeDoc.EVSEMaximumCurrentLimit);
    if (DC_EVSEChargeParameterTypeDoc.EVSEMaximumPowerLimit_isUsed) {
        outJson["EVSEMaximumPowerLimit"] = getJson_PhysicalValueType(DC_EVSEChargeParameterTypeDoc.EVSEMaximumPowerLimit);
    }
    outJson["EVSEMaximumVoltageLimit"] = getJson_PhysicalValueType(DC_EVSEChargeParameterTypeDoc.EVSEMaximumVoltageLimit);
    outJson["EVSEMinimumCurrentLimit"] = getJson_PhysicalValueType(DC_EVSEChargeParameterTypeDoc.EVSEMinimumCurrentLimit);
    outJson["EVSEMinimumVoltageLimit"] = getJson_PhysicalValueType(DC_EVSEChargeParameterTypeDoc.EVSEMinimumVoltageLimit);
    if (DC_EVSEChargeParameterTypeDoc.EVSECurrentRegulationTolerance_isUsed) {
        outJson["EVSECurrentRegulationTolerance"] = getJson_PhysicalValueType(DC_EVSEChargeParameterTypeDoc.EVSECurrentRegulationTolerance);
    }
    outJson["EVSEPeakCurrentRipple"] = getJson_PhysicalValueType(DC_EVSEChargeParameterTypeDoc.EVSEPeakCurrentRipple);
    if (DC_EVSEChargeParameterTypeDoc.EVSEEnergyToBeDelivered_isUsed) {
        outJson["EVSEEnergyToBeDelivered"] = getJson_PhysicalValueType(DC_EVSEChargeParameterTypeDoc.EVSEEnergyToBeDelivered);
    }

    return outJson;
}

struct din_DC_EVSEChargeParameterType getDoc_DC_EVSEChargeParameterType(const json& DC_EVSEChargeParameterTypeJson) {
    struct din_DC_EVSEChargeParameterType outDoc;
    init_din_DC_EVSEChargeParameterType(&outDoc);

    outDoc.DC_EVSEStatus = getDoc_DC_EVSEStatusType(DC_EVSEChargeParameterTypeJson["DC_EVSEStatus"]);
    outDoc.EVSEMaximumCurrentLimit = getDoc_PhysicalValueType(DC_EVSEChargeParameterTypeJson["EVSEMaximumCurrentLimit"]);
    if (DC_EVSEChargeParameterTypeJson.contains("EVSEMaximumPowerLimit")) {
        outDoc.EVSEMaximumPowerLimit_isUsed = 1;
        outDoc.EVSEMaximumPowerLimit = getDoc_PhysicalValueType(DC_EVSEChargeParameterTypeJson["EVSEMaximumPowerLimit"]);
    } else {
        outDoc.EVSEMaximumPowerLimit_isUsed = 0;
    }
    outDoc.EVSEMaximumVoltageLimit = getDoc_PhysicalValueType(DC_EVSEChargeParameterTypeJson["EVSEMaximumVoltageLimit"]);
    outDoc.EVSEMinimumCurrentLimit = getDoc_PhysicalValueType(DC_EVSEChargeParameterTypeJson["EVSEMinimumCurrentLimit"]);
    outDoc.EVSEMinimumVoltageLimit = getDoc_PhysicalValueType(DC_EVSEChargeParameterTypeJson["EVSEMinimumVoltageLimit"]);
    if (DC_EVSEChargeParameterTypeJson.contains("EVSECurrentRegulationTolerance")) {
        outDoc.EVSECurrentRegulationTolerance_isUsed = 1;
        outDoc.EVSECurrentRegulationTolerance = getDoc_PhysicalValueType(DC_EVSEChargeParameterTypeJson["EVSECurrentRegulationTolerance"]);
    } else {
        outDoc.EVSECurrentRegulationTolerance_isUsed = 0;
    }
    outDoc.EVSEPeakCurrentRipple = getDoc_PhysicalValueType(DC_EVSEChargeParameterTypeJson["EVSEPeakCurrentRipple"]);
    if (DC_EVSEChargeParameterTypeJson.contains("EVSEEnergyToBeDelivered")) {
        outDoc.EVSEEnergyToBeDelivered_isUsed = 1;
        outDoc.EVSEEnergyToBeDelivered = getDoc_PhysicalValueType(DC_EVSEChargeParameterTypeJson["EVSEEnergyToBeDelivered"]);
    } else {
        outDoc.EVSEEnergyToBeDelivered_isUsed = 0;
    }

    return outDoc;
}

json getJson_AC_EVSEChargeParameterType(const struct din_AC_EVSEChargeParameterType& AC_EVSEChargeParameterTypeDoc) {
    json outJson;

    outJson["AC_EVSEStatus"] = getJson_AC_EVSEStatusType(AC_EVSEChargeParameterTypeDoc.AC_EVSEStatus);
    outJson["EVSEMaxVoltage"] = getJson_PhysicalValueType(AC_EVSEChargeParameterTypeDoc.EVSEMaxVoltage);
    outJson["EVSEMaxCurrent"] = getJson_PhysicalValueType(AC_EVSEChargeParameterTypeDoc.EVSEMaxCurrent);
    outJson["EVSEMinCurrent"] = getJson_PhysicalValueType(AC_EVSEChargeParameterTypeDoc.EVSEMinCurrent);

    return outJson;
}

struct din_AC_EVSEChargeParameterType getDoc_AC_EVSEChargeParameterType(const json& AC_EVSEChargeParameterTypeJson) {
    struct din_AC_EVSEChargeParameterType outDoc;
    init_din_AC_EVSEChargeParameterType(&outDoc);

    outDoc.AC_EVSEStatus = getDoc_AC_EVSEStatusType(AC_EVSEChargeParameterTypeJson["AC_EVSEStatus"]);
    outDoc.EVSEMaxVoltage = getDoc_PhysicalValueType(AC_EVSEChargeParameterTypeJson["EVSEMaxVoltage"]);
    outDoc.EVSEMaxCurrent = getDoc_PhysicalValueType(AC_EVSEChargeParameterTypeJson["EVSEMaxCurrent"]);
    outDoc.EVSEMinCurrent = getDoc_PhysicalValueType(AC_EVSEChargeParameterTypeJson["EVSEMinCurrent"]);

    return outDoc;
}

json getJson_EVSEChargeParameterType(const struct din_EVSEChargeParameterType& EVSEChargeParameterTypeDoc) {
    json outJson;

    outJson["_unused"] = EVSEChargeParameterTypeDoc._unused;

    return outJson;
}

struct din_EVSEChargeParameterType getDoc_EVSEChargeParameterType(const json& EVSEChargeParameterTypeJson) {
    struct din_EVSEChargeParameterType outDoc;
    init_din_EVSEChargeParameterType(&outDoc);

    outDoc._unused = EVSEChargeParameterTypeJson["_unused"].template get<int>();

    return outDoc;
}

json getJson_MeterInfoType(const struct din_MeterInfoType& MeterInfoTypeDoc) {
    json outJson;

    outJson["MeterID"]["charactersLen"] = MeterInfoTypeDoc.MeterID.charactersLen;
    for (uint16_t i = 0; i < MeterInfoTypeDoc.MeterID.charactersLen; i++) {
        outJson["MeterID"]["characters"][i] = MeterInfoTypeDoc.MeterID.characters[i];
    }
    if (MeterInfoTypeDoc.MeterReading_isUsed) {
        outJson["MeterReading"] = getJson_PhysicalValueType(MeterInfoTypeDoc.MeterReading);
    }
    if (MeterInfoTypeDoc.SigMeterReading_isUsed) {
        outJson["SigMeterReading"]["bytesLen"] = MeterInfoTypeDoc.SigMeterReading.bytesLen;
        for (uint16_t i = 0; i < MeterInfoTypeDoc.SigMeterReading.bytesLen; i++) {
            outJson["SigMeterReading"]["bytes"][i] = MeterInfoTypeDoc.SigMeterReading.bytes[i];
        }
    }
    if (MeterInfoTypeDoc.MeterStatus_isUsed) {
        outJson["MeterStatus"] = MeterInfoTypeDoc.MeterStatus;
    }
    if (MeterInfoTypeDoc.TMeter_isUsed) {
        outJson["TMeter"] = MeterInfoTypeDoc.TMeter;
    }

    return outJson;
}

struct din_MeterInfoType getDoc_MeterInfoType(const json& MeterInfoTypeJson) {
    struct din_MeterInfoType outDoc;
    init_din_MeterInfoType(&outDoc);

    outDoc.MeterID.charactersLen = MeterInfoTypeJson["MeterID"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.MeterID.charactersLen; i++) {
        outDoc.MeterID.characters[i] = MeterInfoTypeJson["MeterID"]["characters"][i].template get<char>();
    }
    if (MeterInfoTypeJson.contains("MeterReading")) {
        outDoc.MeterReading_isUsed = 1;
        outDoc.MeterReading = getDoc_PhysicalValueType(MeterInfoTypeJson["MeterReading"]);
    } else {
        outDoc.MeterReading_isUsed = 0;
    }
    if (MeterInfoTypeJson.contains("SigMeterReading")) {
        outDoc.SigMeterReading_isUsed = 1;
        outDoc.SigMeterReading.bytesLen = MeterInfoTypeJson["SigMeterReading"]["bytesLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.SigMeterReading.bytesLen; i++) {
            outDoc.SigMeterReading.bytes[i] = MeterInfoTypeJson["SigMeterReading"]["bytes"][i].template get<uint8_t>();
        }
    } else {
        outDoc.SigMeterReading_isUsed = 0;
    }
    if (MeterInfoTypeJson.contains("MeterStatus")) {
        outDoc.MeterStatus_isUsed = 1;
        outDoc.MeterStatus = MeterInfoTypeJson["MeterStatus"].template get<int16_t>();
    } else {
        outDoc.MeterStatus_isUsed = 0;
    }
    if (MeterInfoTypeJson.contains("TMeter")) {
        outDoc.TMeter_isUsed = 1;
        outDoc.TMeter = MeterInfoTypeJson["TMeter"].template get<int64_t>();
    } else {
        outDoc.TMeter_isUsed = 0;
    }

    return outDoc;
}

json getJson_CertificateInstallationResType(const struct din_CertificateInstallationResType& CertificateInstallationResTypeDoc) {
    json outJson;

    outJson["Id"]["charactersLen"] = CertificateInstallationResTypeDoc.Id.charactersLen;
    for (uint16_t i = 0; i < CertificateInstallationResTypeDoc.Id.charactersLen; i++) {
        outJson["Id"]["characters"][i] = CertificateInstallationResTypeDoc.Id.characters[i];
    }
    outJson["ResponseCode"] = CertificateInstallationResTypeDoc.ResponseCode;
    outJson["ContractSignatureCertChain"] = getJson_CertificateChainType(CertificateInstallationResTypeDoc.ContractSignatureCertChain);
    outJson["ContractSignatureEncryptedPrivateKey"]["bytesLen"] = CertificateInstallationResTypeDoc.ContractSignatureEncryptedPrivateKey.bytesLen;
    for (uint16_t i = 0; i < CertificateInstallationResTypeDoc.ContractSignatureEncryptedPrivateKey.bytesLen; i++) {
        outJson["ContractSignatureEncryptedPrivateKey"]["bytes"][i] = CertificateInstallationResTypeDoc.ContractSignatureEncryptedPrivateKey.bytes[i];
    }
    outJson["DHParams"]["bytesLen"] = CertificateInstallationResTypeDoc.DHParams.bytesLen;
    for (uint16_t i = 0; i < CertificateInstallationResTypeDoc.DHParams.bytesLen; i++) {
        outJson["DHParams"]["bytes"][i] = CertificateInstallationResTypeDoc.DHParams.bytes[i];
    }
    outJson["ContractID"]["charactersLen"] = CertificateInstallationResTypeDoc.ContractID.charactersLen;
    for (uint16_t i = 0; i < CertificateInstallationResTypeDoc.ContractID.charactersLen; i++) {
        outJson["ContractID"]["characters"][i] = CertificateInstallationResTypeDoc.ContractID.characters[i];
    }

    return outJson;
}

struct din_CertificateInstallationResType getDoc_CertificateInstallationResType(const json& CertificateInstallationResTypeJson) {
    struct din_CertificateInstallationResType outDoc;
    init_din_CertificateInstallationResType(&outDoc);

    outDoc.Id.charactersLen = CertificateInstallationResTypeJson["Id"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
        outDoc.Id.characters[i] = CertificateInstallationResTypeJson["Id"]["characters"][i].template get<char>();
    }
    outDoc.ResponseCode = CertificateInstallationResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.ContractSignatureCertChain = getDoc_CertificateChainType(CertificateInstallationResTypeJson["ContractSignatureCertChain"]);
    outDoc.ContractSignatureEncryptedPrivateKey.bytesLen = CertificateInstallationResTypeJson["ContractSignatureEncryptedPrivateKey"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.ContractSignatureEncryptedPrivateKey.bytesLen; i++) {
        outDoc.ContractSignatureEncryptedPrivateKey.bytes[i] = CertificateInstallationResTypeJson["ContractSignatureEncryptedPrivateKey"]["bytes"][i].template get<uint8_t>();
    }
    outDoc.DHParams.bytesLen = CertificateInstallationResTypeJson["DHParams"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.DHParams.bytesLen; i++) {
        outDoc.DHParams.bytes[i] = CertificateInstallationResTypeJson["DHParams"]["bytes"][i].template get<uint8_t>();
    }
    outDoc.ContractID.charactersLen = CertificateInstallationResTypeJson["ContractID"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.ContractID.charactersLen; i++) {
        outDoc.ContractID.characters[i] = CertificateInstallationResTypeJson["ContractID"]["characters"][i].template get<char>();
    }

    return outDoc;
}

json getJson_CableCheckReqType(const struct din_CableCheckReqType& CableCheckResTypeDoc) {
    json outJson;

    outJson["DC_EVStatus"] = getJson_DC_EVStatusType(CableCheckResTypeDoc.DC_EVStatus);

    return outJson;
}

struct din_CableCheckReqType getDoc_CableCheckReqType(const json& CableCheckResTypeJson) {
    struct din_CableCheckReqType outDoc;
    init_din_CableCheckReqType(&outDoc);

    outDoc.DC_EVStatus = getDoc_DC_EVStatusType(CableCheckResTypeJson["DC_EVStatus"]);

    return outDoc;
}

json getJson_CableCheckResType(const struct din_CableCheckResType& CableCheckResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = CableCheckResTypeDoc.ResponseCode;
    outJson["DC_EVSEStatus"] = getJson_DC_EVSEStatusType(CableCheckResTypeDoc.DC_EVSEStatus);
    outJson["EVSEProcessing"] = CableCheckResTypeDoc.EVSEProcessing;

    return outJson;
}

struct din_CableCheckResType getDoc_CableCheckResType(const json& CableCheckResTypeJson) {
    struct din_CableCheckResType outDoc;
    init_din_CableCheckResType(&outDoc);

    outDoc.ResponseCode = CableCheckResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.DC_EVSEStatus = getDoc_DC_EVSEStatusType(CableCheckResTypeJson["DC_EVSEStatus"]);
    outDoc.EVSEProcessing = CableCheckResTypeJson["EVSEProcessing"].template get<din_EVSEProcessingType>();

    return outDoc;
}

json getJson_PreChargeReqType(const struct din_PreChargeReqType& PreChargeReqTypeDoc) {
    json outJson;

    outJson["DC_EVStatus"] = getJson_DC_EVStatusType(PreChargeReqTypeDoc.DC_EVStatus);
    outJson["EVTargetVoltage"] = getJson_PhysicalValueType(PreChargeReqTypeDoc.EVTargetVoltage);
    outJson["EVTargetCurrent"] = getJson_PhysicalValueType(PreChargeReqTypeDoc.EVTargetCurrent);

    return outJson;
}

struct din_PreChargeReqType getDoc_PreChargeReqType(const json& PreChargeReqTypeJson) {
    struct din_PreChargeReqType outDoc;
    init_din_PreChargeReqType(&outDoc);

    outDoc.DC_EVStatus = getDoc_DC_EVStatusType(PreChargeReqTypeJson["DC_EVStatus"]);
    outDoc.EVTargetVoltage = getDoc_PhysicalValueType(PreChargeReqTypeJson["EVTargetVoltage"]);
    outDoc.EVTargetCurrent = getDoc_PhysicalValueType(PreChargeReqTypeJson["EVTargetCurrent"]);

    return outDoc;
}

json getJson_PreChargeResType(const struct din_PreChargeResType& PreChargeResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = PreChargeResTypeDoc.ResponseCode;
    outJson["DC_EVSEStatus"] = getJson_DC_EVSEStatusType(PreChargeResTypeDoc.DC_EVSEStatus);
    outJson["EVSEPresentVoltage"] = getJson_PhysicalValueType(PreChargeResTypeDoc.EVSEPresentVoltage);

    return outJson;
}

struct din_PreChargeResType getDoc_PreChargeResType(const json& PreChargeResTypeJson) {
    struct din_PreChargeResType outDoc;
    init_din_PreChargeResType(&outDoc);

    outDoc.ResponseCode = PreChargeResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.DC_EVSEStatus = getDoc_DC_EVSEStatusType(PreChargeResTypeJson["DC_EVSEStatus"]);
    outDoc.EVSEPresentVoltage = getDoc_PhysicalValueType(PreChargeResTypeJson["EVSEPresentVoltage"]);

    return outDoc;
}

json getJson_CurrentDemandReqType(const struct din_CurrentDemandReqType& CurrentDemandReqTypeDoc) {
    json outJson;

    outJson["DC_EVStatus"] = getJson_DC_EVStatusType(CurrentDemandReqTypeDoc.DC_EVStatus);
    outJson["EVTargetCurrent"] = getJson_PhysicalValueType(CurrentDemandReqTypeDoc.EVTargetCurrent);
    if (CurrentDemandReqTypeDoc.EVMaximumVoltageLimit_isUsed) {
        outJson["EVMaximumVoltage"] = getJson_PhysicalValueType(CurrentDemandReqTypeDoc.EVMaximumVoltageLimit);
    }
    if (CurrentDemandReqTypeDoc.EVMaximumCurrentLimit_isUsed) {
        outJson["EVMaximumCurrent"] = getJson_PhysicalValueType(CurrentDemandReqTypeDoc.EVMaximumCurrentLimit);
    }
    if (CurrentDemandReqTypeDoc.EVMaximumPowerLimit_isUsed) {
        outJson["EVMaximumPower"] = getJson_PhysicalValueType(CurrentDemandReqTypeDoc.EVMaximumPowerLimit);
    }
    if (CurrentDemandReqTypeDoc.BulkChargingComplete_isUsed) {
        outJson["BulkChargingComplete"] = CurrentDemandReqTypeDoc.BulkChargingComplete;
    }
    outJson["ChargingComplete"] = CurrentDemandReqTypeDoc.ChargingComplete;
    if (CurrentDemandReqTypeDoc.RemainingTimeToFullSoC_isUsed) {
        outJson["RemainingTimeToFullSoC"] = getJson_PhysicalValueType(CurrentDemandReqTypeDoc.RemainingTimeToFullSoC);
    }
    if (CurrentDemandReqTypeDoc.RemainingTimeToBulkSoC_isUsed) {
        outJson["RemainingTimeToBulkSoC"] = getJson_PhysicalValueType(CurrentDemandReqTypeDoc.RemainingTimeToBulkSoC);
    }
    outJson["EVTargetVoltage"] = getJson_PhysicalValueType(CurrentDemandReqTypeDoc.EVTargetVoltage);

    return outJson;
}

struct din_CurrentDemandReqType getDoc_CurrentDemandReqType(const json& CurrentDemandReqTypeJson) {
    struct din_CurrentDemandReqType outDoc;
    init_din_CurrentDemandReqType(&outDoc);

    outDoc.DC_EVStatus = getDoc_DC_EVStatusType(CurrentDemandReqTypeJson["DC_EVStatus"]);
    outDoc.EVTargetCurrent = getDoc_PhysicalValueType(CurrentDemandReqTypeJson["EVTargetCurrent"]);
    if (CurrentDemandReqTypeJson.contains("EVMaximumVoltage")) {
        outDoc.EVMaximumVoltageLimit_isUsed = 1;
        outDoc.EVMaximumVoltageLimit = getDoc_PhysicalValueType(CurrentDemandReqTypeJson["EVMaximumVoltage"]);
    } else {
        outDoc.EVMaximumVoltageLimit_isUsed = 0;
    }
    if (CurrentDemandReqTypeJson.contains("EVMaximumCurrent")) {
        outDoc.EVMaximumCurrentLimit_isUsed = 1;
        outDoc.EVMaximumCurrentLimit = getDoc_PhysicalValueType(CurrentDemandReqTypeJson["EVMaximumCurrent"]);
    } else {
        outDoc.EVMaximumCurrentLimit_isUsed = 0;
    }
    if (CurrentDemandReqTypeJson.contains("EVMaximumPower")) {
        outDoc.EVMaximumPowerLimit_isUsed = 1;
        outDoc.EVMaximumPowerLimit = getDoc_PhysicalValueType(CurrentDemandReqTypeJson["EVMaximumPower"]);
    } else {
        outDoc.EVMaximumPowerLimit_isUsed = 0;
    }
    if (CurrentDemandReqTypeJson.contains("BulkChargingComplete")) {
        outDoc.BulkChargingComplete_isUsed = 1;
        outDoc.BulkChargingComplete = CurrentDemandReqTypeJson["BulkChargingComplete"].template get<int>();
    } else {
        outDoc.BulkChargingComplete_isUsed = 0;
    }
    outDoc.ChargingComplete = CurrentDemandReqTypeJson["ChargingComplete"].template get<int>();
    if (CurrentDemandReqTypeJson.contains("RemainingTimeToFullSoC")) {
        outDoc.RemainingTimeToFullSoC_isUsed = 1;
        outDoc.RemainingTimeToFullSoC = getDoc_PhysicalValueType(CurrentDemandReqTypeJson["RemainingTimeToFullSoC"]);
    } else {
        outDoc.RemainingTimeToFullSoC_isUsed = 0;
    }
    if (CurrentDemandReqTypeJson.contains("RemainingTimeToBulkSoC")) {
        outDoc.RemainingTimeToBulkSoC_isUsed = 1;
        outDoc.RemainingTimeToBulkSoC = getDoc_PhysicalValueType(CurrentDemandReqTypeJson["RemainingTimeToBulkSoC"]);
    } else {
        outDoc.RemainingTimeToBulkSoC_isUsed = 0;
    }
    outDoc.EVTargetVoltage = getDoc_PhysicalValueType(CurrentDemandReqTypeJson["EVTargetVoltage"]);

    return outDoc;
}

json getJson_CurrentDemandResType(const struct din_CurrentDemandResType& CurrentDemandResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = CurrentDemandResTypeDoc.ResponseCode;
    outJson["DC_EVSEStatus"] = getJson_DC_EVSEStatusType(CurrentDemandResTypeDoc.DC_EVSEStatus);
    outJson["EVSEPresentVoltage"] = getJson_PhysicalValueType(CurrentDemandResTypeDoc.EVSEPresentVoltage);
    outJson["EVSEPresentCurrent"] = getJson_PhysicalValueType(CurrentDemandResTypeDoc.EVSEPresentCurrent);
    outJson["EVSECurrentLimitAchieved"] = CurrentDemandResTypeDoc.EVSECurrentLimitAchieved;
    outJson["EVSEVoltageLimitAchieved"] = CurrentDemandResTypeDoc.EVSEVoltageLimitAchieved;
    outJson["EVSEPowerLimitAchieved"] = CurrentDemandResTypeDoc.EVSEPowerLimitAchieved;
    if (CurrentDemandResTypeDoc.EVSEMaximumVoltageLimit_isUsed) {
        outJson["EVSEMaximumVoltageLimit"] = getJson_PhysicalValueType(CurrentDemandResTypeDoc.EVSEMaximumVoltageLimit);
    }
    if (CurrentDemandResTypeDoc.EVSEMaximumCurrentLimit_isUsed) {
        outJson["EVSEMaximumCurrentLimit"] = getJson_PhysicalValueType(CurrentDemandResTypeDoc.EVSEMaximumCurrentLimit);
    }
    if (CurrentDemandResTypeDoc.EVSEMaximumPowerLimit_isUsed) {
        outJson["EVSEMaximumPowerLimit"] = getJson_PhysicalValueType(CurrentDemandResTypeDoc.EVSEMaximumPowerLimit);
    }

    return outJson;
}

struct din_CurrentDemandResType getDoc_CurrentDemandResType(const json& CurrentDemandResTypeJson) {
    struct din_CurrentDemandResType outDoc;
    init_din_CurrentDemandResType(&outDoc);

    outDoc.ResponseCode = CurrentDemandResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.DC_EVSEStatus = getDoc_DC_EVSEStatusType(CurrentDemandResTypeJson["DC_EVSEStatus"]);
    outDoc.EVSEPresentVoltage = getDoc_PhysicalValueType(CurrentDemandResTypeJson["EVSEPresentVoltage"]);
    outDoc.EVSEPresentCurrent = getDoc_PhysicalValueType(CurrentDemandResTypeJson["EVSEPresentCurrent"]);
    outDoc.EVSECurrentLimitAchieved = CurrentDemandResTypeJson["EVSECurrentLimitAchieved"].template get<int>();
    outDoc.EVSEVoltageLimitAchieved = CurrentDemandResTypeJson["EVSEVoltageLimitAchieved"].template get<int>();
    outDoc.EVSEPowerLimitAchieved = CurrentDemandResTypeJson["EVSEPowerLimitAchieved"].template get<int>();
    if (CurrentDemandResTypeJson.contains("EVSEMaximumVoltageLimit")) {
        outDoc.EVSEMaximumVoltageLimit_isUsed = 1;
        outDoc.EVSEMaximumVoltageLimit = getDoc_PhysicalValueType(CurrentDemandResTypeJson["EVSEMaximumVoltageLimit"]);
    } else {
        outDoc.EVSEMaximumVoltageLimit_isUsed = 0;
    }
    if (CurrentDemandResTypeJson.contains("EVSEMaximumCurrentLimit")) {
        outDoc.EVSEMaximumCurrentLimit_isUsed = 1;
        outDoc.EVSEMaximumCurrentLimit = getDoc_PhysicalValueType(CurrentDemandResTypeJson["EVSEMaximumCurrentLimit"]);
    } else {
        outDoc.EVSEMaximumCurrentLimit_isUsed = 0;
    }
    if (CurrentDemandResTypeJson.contains("EVSEMaximumPowerLimit")) {
        outDoc.EVSEMaximumPowerLimit_isUsed = 1;
        outDoc.EVSEMaximumPowerLimit = getDoc_PhysicalValueType(CurrentDemandResTypeJson["EVSEMaximumPowerLimit"]);
    } else {
        outDoc.EVSEMaximumPowerLimit_isUsed = 0;
    }

    return outDoc;
}

json getJson_WeldingDetectionReqType(const struct din_WeldingDetectionReqType& WeldingDetectionReqTypeDoc) {
    json outJson;

    outJson["DC_EVStatus"] = getJson_DC_EVStatusType(WeldingDetectionReqTypeDoc.DC_EVStatus);

    return outJson;
}

struct din_WeldingDetectionReqType getDoc_WeldingDetectionReqType(const json& WeldingDetectionReqTypeJson) {
    struct din_WeldingDetectionReqType outDoc;
    init_din_WeldingDetectionReqType(&outDoc);

    outDoc.DC_EVStatus = getDoc_DC_EVStatusType(WeldingDetectionReqTypeJson["DC_EVStatus"]);

    return outDoc;
}

json getJson_WeldingDetectionResType(const struct din_WeldingDetectionResType& WeldingDetectionResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = WeldingDetectionResTypeDoc.ResponseCode;
    outJson["DC_EVSEStatus"] = getJson_DC_EVSEStatusType(WeldingDetectionResTypeDoc.DC_EVSEStatus);
    outJson["EVSEPresentVoltage"] = getJson_PhysicalValueType(WeldingDetectionResTypeDoc.EVSEPresentVoltage);

    return outJson;
}

struct din_WeldingDetectionResType getDoc_WeldingDetectionResType(const json& WeldingDetectionResTypeJson) {
    struct din_WeldingDetectionResType outDoc;
    init_din_WeldingDetectionResType(&outDoc);

    outDoc.ResponseCode = WeldingDetectionResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.DC_EVSEStatus = getDoc_DC_EVSEStatusType(WeldingDetectionResTypeJson["DC_EVSEStatus"]);
    outDoc.EVSEPresentVoltage = getDoc_PhysicalValueType(WeldingDetectionResTypeJson["EVSEPresentVoltage"]);

    return outDoc;
}

json getJson_SessionSetupReqType(const struct din_SessionSetupReqType& SessionSetupReqTypeDoc) {
    json outJson;

    outJson["EVCCID"]["bytesLen"] = SessionSetupReqTypeDoc.EVCCID.bytesLen;
    for (uint8_t i = 0; i < SessionSetupReqTypeDoc.EVCCID.bytesLen; i++) {
        outJson["EVCCID"]["bytes"][i] = SessionSetupReqTypeDoc.EVCCID.bytes[i];
    }

    return outJson;
}

struct din_SessionSetupReqType getDoc_SessionSetupReqType(const json& SessionSetupReqTypeJson) {
    struct din_SessionSetupReqType outDoc;
    init_din_SessionSetupReqType(&outDoc);

    outDoc.EVCCID.bytesLen = SessionSetupReqTypeJson["EVCCID"]["bytesLen"].template get<uint8_t>();
    for (uint8_t i = 0; i < outDoc.EVCCID.bytesLen; i++) {
        outDoc.EVCCID.bytes[i] = SessionSetupReqTypeJson["EVCCID"]["bytes"][i].template get<uint8_t>();
    }

    return outDoc;
}

json getJson_CertificateInstallationReqType(const struct din_CertificateInstallationReqType& CertificateInstallationReqTypeDoc) {
    json outJson;

    if (CertificateInstallationReqTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = CertificateInstallationReqTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < CertificateInstallationReqTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = CertificateInstallationReqTypeDoc.Id.characters[i];
        }
    }
    outJson["OEMProvisioningCert"]["bytesLen"] = CertificateInstallationReqTypeDoc.OEMProvisioningCert.bytesLen;
    for (uint16_t i = 0; i < CertificateInstallationReqTypeDoc.OEMProvisioningCert.bytesLen; i++) {
        outJson["OEMProvisioningCert"]["bytes"][i] = CertificateInstallationReqTypeDoc.OEMProvisioningCert.bytes[i];
    }
    outJson["ListOfRootCertificateIDs"] = getJson_ListOfRootCertificateIDsType(CertificateInstallationReqTypeDoc.ListOfRootCertificateIDs);
    outJson["DHParams"]["bytesLen"] = CertificateInstallationReqTypeDoc.DHParams.bytesLen;
    for (uint16_t i = 0; i < CertificateInstallationReqTypeDoc.DHParams.bytesLen; i++) {
        outJson["DHParams"]["bytes"][i] = CertificateInstallationReqTypeDoc.DHParams.bytes[i];
    }

    return outJson;
}

struct din_CertificateInstallationReqType getDoc_CertificateInstallationReqType(const json& CertificateInstallationReqTypeJson) {
    struct din_CertificateInstallationReqType outDoc;
    init_din_CertificateInstallationReqType(&outDoc);

    if (CertificateInstallationReqTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = CertificateInstallationReqTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = CertificateInstallationReqTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    outDoc.OEMProvisioningCert.bytesLen = CertificateInstallationReqTypeJson["OEMProvisioningCert"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.OEMProvisioningCert.bytesLen; i++) {
        outDoc.OEMProvisioningCert.bytes[i] = CertificateInstallationReqTypeJson["OEMProvisioningCert"]["bytes"][i].template get<uint8_t>();
    }
    outDoc.ListOfRootCertificateIDs = getDoc_ListOfRootCertificateIDsType(CertificateInstallationReqTypeJson["ListOfRootCertificateIDs"]);
    outDoc.DHParams.bytesLen = CertificateInstallationReqTypeJson["DHParams"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.DHParams.bytesLen; i++) {
        outDoc.DHParams.bytes[i] = CertificateInstallationReqTypeJson["DHParams"]["bytes"][i].template get<uint8_t>();
    }

    return outDoc;
}

json getJson_SessionSetupResType(const struct din_SessionSetupResType& SessionSetupResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = SessionSetupResTypeDoc.ResponseCode;
    outJson["EVSEID"]["bytesLen"] = SessionSetupResTypeDoc.EVSEID.bytesLen;
    for (uint8_t i = 0; i < SessionSetupResTypeDoc.EVSEID.bytesLen; i++) {
        outJson["EVSEID"]["bytes"][i] = SessionSetupResTypeDoc.EVSEID.bytes[i];
    }
    if (SessionSetupResTypeDoc.DateTimeNow_isUsed) {
        outJson["DateTimeNow"] = SessionSetupResTypeDoc.DateTimeNow;
    }

    return outJson;
}

struct din_SessionSetupResType getDoc_SessionSetupResType(const json& SessionSetupResTypeJson) {
    struct din_SessionSetupResType outDoc;
    init_din_SessionSetupResType(&outDoc);

    outDoc.ResponseCode = SessionSetupResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.EVSEID.bytesLen = SessionSetupResTypeJson["EVSEID"]["bytesLen"].template get<uint16_t>();
    for (uint8_t i = 0; i < outDoc.EVSEID.bytesLen; i++) {
        outDoc.EVSEID.bytes[i] = SessionSetupResTypeJson["EVSEID"]["bytes"][i].template get<uint8_t>();
    }
    if (SessionSetupResTypeJson.contains("DateTimeNow")) {
        outDoc.DateTimeNow_isUsed = 1;
        outDoc.DateTimeNow = SessionSetupResTypeJson["DateTimeNow"].template get<int64_t>();
    } else {
        outDoc.DateTimeNow_isUsed = 0;
    }

    return outDoc;
}

json getJson_ServiceDiscoveryReqType(const struct din_ServiceDiscoveryReqType& ServiceDiscoveryReqTypeDoc) {
    json outJson;

    if (ServiceDiscoveryReqTypeDoc.ServiceScope_isUsed) {
        outJson["ServiceScope"]["charactersLen"] = ServiceDiscoveryReqTypeDoc.ServiceScope.charactersLen;
        for (uint16_t i = 0; i < ServiceDiscoveryReqTypeDoc.ServiceScope.charactersLen; i++) {
            outJson["ServiceScope"]["characters"][i] = ServiceDiscoveryReqTypeDoc.ServiceScope.characters[i];
        }
    }
    if (ServiceDiscoveryReqTypeDoc.ServiceCategory_isUsed) {
        outJson["ServiceCategory"] = ServiceDiscoveryReqTypeDoc.ServiceCategory;
    }

    return outJson;
}

struct din_ServiceDiscoveryReqType getDoc_ServiceDiscoveryReqType(const json& ServiceDiscoveryReqTypeJson) {
    struct din_ServiceDiscoveryReqType outDoc;
    init_din_ServiceDiscoveryReqType(&outDoc);

    if (ServiceDiscoveryReqTypeJson.contains("ServiceScope")) {
        outDoc.ServiceScope_isUsed = 1;
        outDoc.ServiceScope.charactersLen = ServiceDiscoveryReqTypeJson["ServiceScope"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.ServiceScope.charactersLen; i++) {
            outDoc.ServiceScope.characters[i] = ServiceDiscoveryReqTypeJson["ServiceScope"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.ServiceScope_isUsed = 0;
    }
    if (ServiceDiscoveryReqTypeJson.contains("ServiceCategory")) {
        outDoc.ServiceCategory_isUsed = 1;
        outDoc.ServiceCategory = ServiceDiscoveryReqTypeJson["ServiceCategory"].template get<din_serviceCategoryType>();
    } else {
        outDoc.ServiceCategory_isUsed = 0;
    }

    return outDoc;
}

json getJson_ServiceDiscoveryResType(const struct din_ServiceDiscoveryResType& ServiceDiscoveryResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = ServiceDiscoveryResTypeDoc.ResponseCode;
    outJson["PaymentOptions"] = getJson_PaymentOptionsType(ServiceDiscoveryResTypeDoc.PaymentOptions);
    outJson["ChargeService"] = getJson_ServiceChargeType(ServiceDiscoveryResTypeDoc.ChargeService);
    if (ServiceDiscoveryResTypeDoc.ServiceList_isUsed) {
        outJson["ServiceList"] = getJson_ServiceTagListType(ServiceDiscoveryResTypeDoc.ServiceList);
    }

    return outJson;
}

struct din_ServiceDiscoveryResType getDoc_ServiceDiscoveryResType(const json& ServiceDiscoveryResTypeJson) {
    struct din_ServiceDiscoveryResType outDoc;
    init_din_ServiceDiscoveryResType(&outDoc);

    outDoc.ResponseCode = ServiceDiscoveryResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.PaymentOptions = getDoc_PaymentOptionsType(ServiceDiscoveryResTypeJson["PaymentOptions"]);
    outDoc.ChargeService = getDoc_ServiceChargeType(ServiceDiscoveryResTypeJson["ChargeService"]);
    if (ServiceDiscoveryResTypeJson.contains("ServiceList")) {
        outDoc.ServiceList_isUsed = 1;
        outDoc.ServiceList = getDoc_ServiceTagListType(ServiceDiscoveryResTypeJson["ServiceList"]);
    } else {
        outDoc.ServiceList_isUsed = 0;
    }

    return outDoc;
}

json getJson_ServiceDetailReqType(const struct din_ServiceDetailReqType& ServiceDetailReqTypeDoc) {
    json outJson;

    outJson["ServiceID"] = ServiceDetailReqTypeDoc.ServiceID;

    return outJson;
}

struct din_ServiceDetailReqType getDoc_ServiceDetailReqType(const json& ServiceDetailReqTypeJson) {
    struct din_ServiceDetailReqType outDoc;
    init_din_ServiceDetailReqType(&outDoc);

    outDoc.ServiceID = ServiceDetailReqTypeJson["ServiceID"].template get<uint16_t>();

    return outDoc;
}

json getJson_ServiceDetailResType(const struct din_ServiceDetailResType& ServiceDetailResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = ServiceDetailResTypeDoc.ResponseCode;
    outJson["ServiceID"] = ServiceDetailResTypeDoc.ServiceID;
    if (ServiceDetailResTypeDoc.ServiceParameterList_isUsed) {
        outJson["ServiceParameterList"] = getJson_ServiceParameterListType(ServiceDetailResTypeDoc.ServiceParameterList);
    }

    return outJson;
}

struct din_ServiceDetailResType getDoc_ServiceDetailResType(const json& ServiceDetailResTypeJson) {
    struct din_ServiceDetailResType outDoc;
    init_din_ServiceDetailResType(&outDoc);

    outDoc.ResponseCode = ServiceDetailResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.ServiceID = ServiceDetailResTypeJson["ServiceID"].template get<uint16_t>();
    if (ServiceDetailResTypeJson.contains("ServiceParameterList")) {
        outDoc.ServiceParameterList_isUsed = 1;
        outDoc.ServiceParameterList = getDoc_ServiceParameterListType(ServiceDetailResTypeJson["ServiceParameterList"]);
    } else {
        outDoc.ServiceParameterList_isUsed = 0;
    }

    return outDoc;
}

json getJson_ServicePaymentSelectionReqType(const struct din_ServicePaymentSelectionReqType& ServicePaymentSelectionReqTypeDoc) {
    json outJson;

    outJson["SelectedPaymentOption"] = ServicePaymentSelectionReqTypeDoc.SelectedPaymentOption;
    outJson["SelectedServiceList"] = getJson_SelectedServiceListType(ServicePaymentSelectionReqTypeDoc.SelectedServiceList);

    return outJson;
}

struct din_ServicePaymentSelectionReqType getDoc_ServicePaymentSelectionReqType(const json& ServicePaymentSelectionReqTypeJson) {
    struct din_ServicePaymentSelectionReqType outDoc;
    init_din_ServicePaymentSelectionReqType(&outDoc);

    outDoc.SelectedPaymentOption = ServicePaymentSelectionReqTypeJson["SelectedPaymentOption"].template get<din_paymentOptionType>();
    outDoc.SelectedServiceList = getDoc_SelectedServiceListType(ServicePaymentSelectionReqTypeJson["SelectedServiceList"]);

    return outDoc;
}

json getJson_ServicePaymentSelectionResType(const struct din_ServicePaymentSelectionResType& ServicePaymentSelectionResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = ServicePaymentSelectionResTypeDoc.ResponseCode;

    return outJson;
}

struct din_ServicePaymentSelectionResType getDoc_ServicePaymentSelectionResType(const json& ServicePaymentSelectionResTypeJson) {
    struct din_ServicePaymentSelectionResType outDoc;
    init_din_ServicePaymentSelectionResType(&outDoc);

    outDoc.ResponseCode = ServicePaymentSelectionResTypeJson["ResponseCode"].template get<din_responseCodeType>();

    return outDoc;
}

json getJson_PaymentDetailsReqType(const struct din_PaymentDetailsReqType& PaymentDetailsReqTypeDoc) {
    json outJson;

    outJson["ContractID"]["charactersLen"] = PaymentDetailsReqTypeDoc.ContractID.charactersLen;
    for (uint16_t i = 0; i < PaymentDetailsReqTypeDoc.ContractID.charactersLen; i++) {
        outJson["ContractID"]["characters"][i] = PaymentDetailsReqTypeDoc.ContractID.characters[i];
    }
    outJson["ContractSignatureCertChain"] = getJson_CertificateChainType(PaymentDetailsReqTypeDoc.ContractSignatureCertChain);

    return outJson;
}

struct din_PaymentDetailsReqType getDoc_PaymentDetailsReqType(const json& PaymentDetailsReqTypeJson) {
    struct din_PaymentDetailsReqType outDoc;
    init_din_PaymentDetailsReqType(&outDoc);

    outDoc.ContractID.charactersLen = PaymentDetailsReqTypeJson["ContractID"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.ContractID.charactersLen; i++) {
        outDoc.ContractID.characters[i] = PaymentDetailsReqTypeJson["ContractID"]["characters"][i].template get<char>();
    }
    outDoc.ContractSignatureCertChain = getDoc_CertificateChainType(PaymentDetailsReqTypeJson["ContractSignatureCertChain"]);

    return outDoc;
}

json getJson_PaymentDetailsResType(const struct din_PaymentDetailsResType& PaymentDetailsResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = PaymentDetailsResTypeDoc.ResponseCode;
    outJson["GenChallenge"]["charactersLen"] = PaymentDetailsResTypeDoc.GenChallenge.charactersLen;
    for (uint16_t i = 0; i < PaymentDetailsResTypeDoc.GenChallenge.charactersLen; i++) {
        outJson["GenChallenge"]["characters"][i] = PaymentDetailsResTypeDoc.GenChallenge.characters[i];
    }
    outJson["DateTimeNow"] = PaymentDetailsResTypeDoc.DateTimeNow;

    return outJson;
}

struct din_PaymentDetailsResType getDoc_PaymentDetailsResType(const json& PaymentDetailsResTypeJson) {
    struct din_PaymentDetailsResType outDoc;
    init_din_PaymentDetailsResType(&outDoc);

    outDoc.ResponseCode = PaymentDetailsResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.GenChallenge.charactersLen = PaymentDetailsResTypeJson["GenChallenge"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.GenChallenge.charactersLen; i++) {
        outDoc.GenChallenge.characters[i] = PaymentDetailsResTypeJson["GenChallenge"]["characters"][i].template get<char>();
    }
    outDoc.DateTimeNow = PaymentDetailsResTypeJson["DateTimeNow"].template get<int64_t>();

    return outDoc;
}

json getJson_ContractAuthenticationReqType(const struct din_ContractAuthenticationReqType& ContractAuthenticationReqTypeDoc) {
    json outJson;

    if (ContractAuthenticationReqTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = ContractAuthenticationReqTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < ContractAuthenticationReqTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = ContractAuthenticationReqTypeDoc.Id.characters[i];
        }
    }
    if (ContractAuthenticationReqTypeDoc.GenChallenge_isUsed) {
        outJson["GenChallenge"]["charactersLen"] = ContractAuthenticationReqTypeDoc.GenChallenge.charactersLen;
        for (uint16_t i = 0; i < ContractAuthenticationReqTypeDoc.GenChallenge.charactersLen; i++) {
            outJson["GenChallenge"]["characters"][i] = ContractAuthenticationReqTypeDoc.GenChallenge.characters[i];
        }
    }

    return outJson;
}

struct din_ContractAuthenticationReqType getDoc_ContractAuthenticationReqType(const json& ContractAuthenticationReqTypeJson) {
    struct din_ContractAuthenticationReqType outDoc;
    init_din_ContractAuthenticationReqType(&outDoc);

    if (ContractAuthenticationReqTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = ContractAuthenticationReqTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = ContractAuthenticationReqTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    if (ContractAuthenticationReqTypeJson.contains("GenChallenge")) {
        outDoc.GenChallenge_isUsed = 1;
        outDoc.GenChallenge.charactersLen = ContractAuthenticationReqTypeJson["GenChallenge"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.GenChallenge.charactersLen; i++) {
            outDoc.GenChallenge.characters[i] = ContractAuthenticationReqTypeJson["GenChallenge"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.GenChallenge_isUsed = 0;
    }

    return outDoc;
}

json getJson_ContractAuthenticationResType(const struct din_ContractAuthenticationResType& ContractAuthenticationResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = ContractAuthenticationResTypeDoc.ResponseCode;
    outJson["EVSEProcessing"] = ContractAuthenticationResTypeDoc.EVSEProcessing;

    return outJson;
}

struct din_ContractAuthenticationResType getDoc_ContractAuthenticationResType(const json& ContractAuthenticationResTypeJson) {
    struct din_ContractAuthenticationResType outDoc;
    init_din_ContractAuthenticationResType(&outDoc);

    outDoc.ResponseCode = ContractAuthenticationResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.EVSEProcessing = ContractAuthenticationResTypeJson["EVSEProcessing"].template get<din_EVSEProcessingType>();

    return outDoc;
}

json getJson_ChargeParameterDiscoveryReqType(const struct din_ChargeParameterDiscoveryReqType& ChargeParameterDiscoveryReqTypeDoc) {
    json outJson;

    outJson["EVRequestedEnergyTransferType"] = ChargeParameterDiscoveryReqTypeDoc.EVRequestedEnergyTransferType;
    if (ChargeParameterDiscoveryReqTypeDoc.AC_EVChargeParameter_isUsed) {
        outJson["AC_EVChargeParameter"] = getJson_AC_EVChargeParameterType(ChargeParameterDiscoveryReqTypeDoc.AC_EVChargeParameter);
    }
    if (ChargeParameterDiscoveryReqTypeDoc.DC_EVChargeParameter_isUsed) {
        outJson["DC_EVChargeParameter"] = getJson_DC_EVChargeParameterType(ChargeParameterDiscoveryReqTypeDoc.DC_EVChargeParameter);
    }
    if (ChargeParameterDiscoveryReqTypeDoc.EVChargeParameter_isUsed) {
        outJson["EVChargeParameter"] = getJson_EVChargeParameterType(ChargeParameterDiscoveryReqTypeDoc.EVChargeParameter);
    }

    return outJson;
}

struct din_ChargeParameterDiscoveryReqType getDoc_ChargeParameterDiscoveryReqType(const json& ChargeParameterDiscoveryReqTypeJson) {
    struct din_ChargeParameterDiscoveryReqType outDoc;
    init_din_ChargeParameterDiscoveryReqType(&outDoc);

    outDoc.EVRequestedEnergyTransferType = ChargeParameterDiscoveryReqTypeJson["EVRequestedEnergyTransferType"].template get<din_EVRequestedEnergyTransferType>();
    if (ChargeParameterDiscoveryReqTypeJson.contains("AC_EVChargeParameter")) {
        outDoc.AC_EVChargeParameter_isUsed = 1;
        outDoc.AC_EVChargeParameter = getDoc_AC_EVChargeParameterType(ChargeParameterDiscoveryReqTypeJson["AC_EVChargeParameter"]);
    } else {
        outDoc.AC_EVChargeParameter_isUsed = 0;
    }
    if (ChargeParameterDiscoveryReqTypeJson.contains("DC_EVChargeParameter")) {
        outDoc.DC_EVChargeParameter_isUsed = 1;
        outDoc.DC_EVChargeParameter = getDoc_DC_EVChargeParameterType(ChargeParameterDiscoveryReqTypeJson["DC_EVChargeParameter"]);
    } else {
        outDoc.DC_EVChargeParameter_isUsed = 0;
    }
    if (ChargeParameterDiscoveryReqTypeJson.contains("EVChargeParameter")) {
        outDoc.EVChargeParameter_isUsed = 1;
        outDoc.EVChargeParameter = getDoc_EVChargeParameterType(ChargeParameterDiscoveryReqTypeJson["EVChargeParameter"]);
    } else {
        outDoc.EVChargeParameter_isUsed = 0;
    }

    return outDoc;
}

json getJson_ChargeParameterDiscoveryResType(const struct din_ChargeParameterDiscoveryResType& ChargeParameterDiscoveryResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = ChargeParameterDiscoveryResTypeDoc.ResponseCode;
    outJson["EVSEProcessing"] = ChargeParameterDiscoveryResTypeDoc.EVSEProcessing;
    if (ChargeParameterDiscoveryResTypeDoc.SAScheduleList_isUsed) {
        outJson["SAScheduleList"] = getJson_SAScheduleListType(ChargeParameterDiscoveryResTypeDoc.SAScheduleList);
    }
    if (ChargeParameterDiscoveryResTypeDoc.SASchedules_isUsed) {
        outJson["SASchedules"] = getJson_SASchedulesType(ChargeParameterDiscoveryResTypeDoc.SASchedules);
    }
    if (ChargeParameterDiscoveryResTypeDoc.AC_EVSEChargeParameter_isUsed) {
        outJson["AC_EVSEChargeParameter"] = getJson_AC_EVSEChargeParameterType(ChargeParameterDiscoveryResTypeDoc.AC_EVSEChargeParameter);
    }
    if (ChargeParameterDiscoveryResTypeDoc.DC_EVSEChargeParameter_isUsed) {
        outJson["DC_EVSEChargeParameter"] = getJson_DC_EVSEChargeParameterType(ChargeParameterDiscoveryResTypeDoc.DC_EVSEChargeParameter);
    }
    if (ChargeParameterDiscoveryResTypeDoc.EVSEChargeParameter_isUsed) {
        outJson["EVSEChargeParameter"] = getJson_EVSEChargeParameterType(ChargeParameterDiscoveryResTypeDoc.EVSEChargeParameter);
    }

    return outJson;
}

struct din_ChargeParameterDiscoveryResType getDoc_ChargeParameterDiscoveryResType(const json& ChargeParameterDiscoveryResTypeJson) {
    struct din_ChargeParameterDiscoveryResType outDoc;
    init_din_ChargeParameterDiscoveryResType(&outDoc);

    outDoc.ResponseCode = ChargeParameterDiscoveryResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.EVSEProcessing = ChargeParameterDiscoveryResTypeJson["EVSEProcessing"].template get<din_EVSEProcessingType>();
    if (ChargeParameterDiscoveryResTypeJson.contains("SAScheduleList")) {
        outDoc.SAScheduleList_isUsed = 1;
        outDoc.SAScheduleList = getDoc_SAScheduleListType(ChargeParameterDiscoveryResTypeJson["SAScheduleList"]);
    } else {
        outDoc.SAScheduleList_isUsed = 0;
    }
    if (ChargeParameterDiscoveryResTypeJson.contains("SASchedules")) {
        outDoc.SASchedules_isUsed = 1;
        outDoc.SASchedules = getDoc_SASchedulesType(ChargeParameterDiscoveryResTypeJson["SASchedules"]);
    } else {
        outDoc.SASchedules_isUsed = 0;
    }
    if (ChargeParameterDiscoveryResTypeJson.contains("AC_EVSEChargeParameter")) {
        outDoc.AC_EVSEChargeParameter_isUsed = 1;
        outDoc.AC_EVSEChargeParameter = getDoc_AC_EVSEChargeParameterType(ChargeParameterDiscoveryResTypeJson["AC_EVSEChargeParameter"]);
    } else {
        outDoc.AC_EVSEChargeParameter_isUsed = 0;
    }
    if (ChargeParameterDiscoveryResTypeJson.contains("DC_EVSEChargeParameter")) {
        outDoc.DC_EVSEChargeParameter_isUsed = 1;
        outDoc.DC_EVSEChargeParameter = getDoc_DC_EVSEChargeParameterType(ChargeParameterDiscoveryResTypeJson["DC_EVSEChargeParameter"]);
    } else {
        outDoc.DC_EVSEChargeParameter_isUsed = 0;
    }
    if (ChargeParameterDiscoveryResTypeJson.contains("EVSEChargeParameter")) {
        outDoc.EVSEChargeParameter_isUsed = 1;
        outDoc.EVSEChargeParameter = getDoc_EVSEChargeParameterType(ChargeParameterDiscoveryResTypeJson["EVSEChargeParameter"]);
    } else {
        outDoc.EVSEChargeParameter_isUsed = 0;
    }

    return outDoc;
}

json getJson_PowerDeliveryReqType(const struct din_PowerDeliveryReqType& PowerDeliveryReqTypeDoc) {
    json outJson;

    outJson["ReadyToChargeState"] = PowerDeliveryReqTypeDoc.ReadyToChargeState;
    if (PowerDeliveryReqTypeDoc.ChargingProfile_isUsed) {
        outJson["ChargingProfile"] = getJson_ChargingProfileType(PowerDeliveryReqTypeDoc.ChargingProfile);
    }
    if (PowerDeliveryReqTypeDoc.DC_EVPowerDeliveryParameter_isUsed) {
        outJson["DC_EVPowerDeliveryParameter"] = getJson_DC_EVPowerDeliveryParameterType(PowerDeliveryReqTypeDoc.DC_EVPowerDeliveryParameter);
    }
    if (PowerDeliveryReqTypeDoc.EVPowerDeliveryParameter_isUsed) {
        outJson["EVPowerDeliveryParameter"] = getJson_EVPowerDeliveryParameterType(PowerDeliveryReqTypeDoc.EVPowerDeliveryParameter);
    }

    return outJson;
}

struct din_PowerDeliveryReqType getDoc_PowerDeliveryReqType(const json& PowerDeliveryReqTypeJson) {
    struct din_PowerDeliveryReqType outDoc;
    init_din_PowerDeliveryReqType(&outDoc);

    outDoc.ReadyToChargeState = PowerDeliveryReqTypeJson["ReadyToChargeState"].template get<int>();
    if (PowerDeliveryReqTypeJson.contains("ChargingProfile")) {
        outDoc.ChargingProfile_isUsed = 1;
        outDoc.ChargingProfile = getDoc_ChargingProfileType(PowerDeliveryReqTypeJson["ChargingProfile"]);
    } else {
        outDoc.ChargingProfile_isUsed = 0;
    }
    if (PowerDeliveryReqTypeJson.contains("DC_EVPowerDeliveryParameter")) {
        outDoc.DC_EVPowerDeliveryParameter_isUsed = 1;
        outDoc.DC_EVPowerDeliveryParameter = getDoc_DC_EVPowerDeliveryParameterType(PowerDeliveryReqTypeJson["DC_EVPowerDeliveryParameter"]);
    } else {
        outDoc.DC_EVPowerDeliveryParameter_isUsed = 0;
    }
    if (PowerDeliveryReqTypeJson.contains("EVPowerDeliveryParameter")) {
        outDoc.EVPowerDeliveryParameter_isUsed = 1;
        outDoc.EVPowerDeliveryParameter = getDoc_EVPowerDeliveryParameterType(PowerDeliveryReqTypeJson["EVPowerDeliveryParameter"]);
    } else {
        outDoc.EVPowerDeliveryParameter_isUsed = 0;
    }

    return outDoc;
}

json getJson_PowerDeliveryResType(const struct din_PowerDeliveryResType& PowerDeliveryResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = PowerDeliveryResTypeDoc.ResponseCode;
    if (PowerDeliveryResTypeDoc.AC_EVSEStatus_isUsed) {
        outJson["AC_EVSEStatus"] = getJson_AC_EVSEStatusType(PowerDeliveryResTypeDoc.AC_EVSEStatus);
    }
    if (PowerDeliveryResTypeDoc.DC_EVSEStatus_isUsed) {
        outJson["DC_EVSEStatus"] = getJson_DC_EVSEStatusType(PowerDeliveryResTypeDoc.DC_EVSEStatus);
    }
    if (PowerDeliveryResTypeDoc.EVSEStatus_isUsed) {
        outJson["EVSEStatus"] = getJson_EVSEStatusType(PowerDeliveryResTypeDoc.EVSEStatus);
    }

    return outJson;
}

struct din_PowerDeliveryResType getDoc_PowerDeliveryResType(const json& PowerDeliveryResTypeJson) {
    struct din_PowerDeliveryResType outDoc;
    init_din_PowerDeliveryResType(&outDoc);

    outDoc.ResponseCode = PowerDeliveryResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    if (PowerDeliveryResTypeJson.contains("AC_EVSEStatus")) {
        outDoc.AC_EVSEStatus_isUsed = 1;
        outDoc.AC_EVSEStatus = getDoc_AC_EVSEStatusType(PowerDeliveryResTypeJson["AC_EVSEStatus"]);
    } else {
        outDoc.AC_EVSEStatus_isUsed = 0;
    }
    if (PowerDeliveryResTypeJson.contains("DC_EVSEStatus")) {
        outDoc.DC_EVSEStatus_isUsed = 1;
        outDoc.DC_EVSEStatus = getDoc_DC_EVSEStatusType(PowerDeliveryResTypeJson["DC_EVSEStatus"]);
    } else {
        outDoc.DC_EVSEStatus_isUsed = 0;
    }
    if (PowerDeliveryResTypeJson.contains("EVSEStatus")) {
        outDoc.EVSEStatus_isUsed = 1;
        outDoc.EVSEStatus = getDoc_EVSEStatusType(PowerDeliveryResTypeJson["EVSEStatus"]);
    } else {
        outDoc.EVSEStatus_isUsed = 0;
    }

    return outDoc;
}

json getJson_ChargingStatusReqType(const struct din_ChargingStatusReqType& ChargingStatusReqTypeDoc) {
    json outJson;

    outJson["_unused"] = ChargingStatusReqTypeDoc._unused;

    return outJson;
}

struct din_ChargingStatusReqType getDoc_ChargingStatusReqType(const json& ChargingStatusReqTypeJson) {
    struct din_ChargingStatusReqType outDoc;
    init_din_ChargingStatusReqType(&outDoc);

    outDoc._unused = ChargingStatusReqTypeJson["_unused"].template get<int>();

    return outDoc;
}

json getJson_ChargingStatusResType(const struct din_ChargingStatusResType& ChargingStatusResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = ChargingStatusResTypeDoc.ResponseCode;
    outJson["EVSEID"]["bytesLen"] = ChargingStatusResTypeDoc.EVSEID.bytesLen;
    for (uint16_t i = 0; i < ChargingStatusResTypeDoc.EVSEID.bytesLen; i++) {
        outJson["EVSEID"]["bytes"][i] = ChargingStatusResTypeDoc.EVSEID.bytes[i];
    }
    outJson["SAScheduleTupleID"] = ChargingStatusResTypeDoc.SAScheduleTupleID;
    if (ChargingStatusResTypeDoc.EVSEMaxCurrent_isUsed) {
        outJson["EVSEMaxCurrent"] = getJson_PhysicalValueType(ChargingStatusResTypeDoc.EVSEMaxCurrent);
    }
    if (ChargingStatusResTypeDoc.MeterInfo_isUsed) {
        outJson["MeterInfo"] = getJson_MeterInfoType(ChargingStatusResTypeDoc.MeterInfo);
    }
    outJson["ReceiptRequired"] = ChargingStatusResTypeDoc.ReceiptRequired;
    outJson["AC_EVSEStatus"] = getJson_AC_EVSEStatusType(ChargingStatusResTypeDoc.AC_EVSEStatus);

    return outJson;
}

struct din_ChargingStatusResType getDoc_ChargingStatusResType(const json& ChargingStatusResTypeJson) {
    struct din_ChargingStatusResType outDoc;
    init_din_ChargingStatusResType(&outDoc);

    outDoc.ResponseCode = ChargingStatusResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.EVSEID.bytesLen = ChargingStatusResTypeJson["EVSEID"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.EVSEID.bytesLen; i++) {
        outDoc.EVSEID.bytes[i] = ChargingStatusResTypeJson["EVSEID"]["bytes"][i].template get<uint8_t>();
    }
    outDoc.SAScheduleTupleID = ChargingStatusResTypeJson["SAScheduleTupleID"].template get<int16_t>();
    if (ChargingStatusResTypeJson.contains("EVSEMaxCurrent")) {
        outDoc.EVSEMaxCurrent_isUsed = 1;
        outDoc.EVSEMaxCurrent = getDoc_PhysicalValueType(ChargingStatusResTypeJson["EVSEMaxCurrent"]);
    } else {
        outDoc.EVSEMaxCurrent_isUsed = 0;
    }
    if (ChargingStatusResTypeJson.contains("MeterInfo")) {
        outDoc.MeterInfo_isUsed = 1;
        outDoc.MeterInfo = getDoc_MeterInfoType(ChargingStatusResTypeJson["MeterInfo"]);
    } else {
        outDoc.MeterInfo_isUsed = 0;
    }
    outDoc.ReceiptRequired = ChargingStatusResTypeJson["ReceiptRequired"].template get<int>();
    outDoc.AC_EVSEStatus = getDoc_AC_EVSEStatusType(ChargingStatusResTypeJson["AC_EVSEStatus"]);

    return outDoc;
}

json getJson_MeteringReceiptReqType(const struct din_MeteringReceiptReqType& MeteringReceiptReqTypeDoc) {
    json outJson;

    if (MeteringReceiptReqTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = MeteringReceiptReqTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < MeteringReceiptReqTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = MeteringReceiptReqTypeDoc.Id.characters[i];
        }
    }
    outJson["SessionID"]["bytesLen"] = MeteringReceiptReqTypeDoc.SessionID.bytesLen;
    for (uint16_t i = 0; i < MeteringReceiptReqTypeDoc.SessionID.bytesLen; i++) {
        outJson["SessionID"]["bytes"][i] = MeteringReceiptReqTypeDoc.SessionID.bytes[i];
    }
    if (MeteringReceiptReqTypeDoc.SAScheduleTupleID_isUsed) {
        outJson["SAScheduleTupleID"] = MeteringReceiptReqTypeDoc.SAScheduleTupleID;
    }
    outJson["MeterInfo"] = getJson_MeterInfoType(MeteringReceiptReqTypeDoc.MeterInfo);

    return outJson;
}

struct din_MeteringReceiptReqType getDoc_MeteringReceiptReqType(const json& MeteringReceiptReqTypeJson) {
    struct din_MeteringReceiptReqType outDoc;
    init_din_MeteringReceiptReqType(&outDoc);

    if (MeteringReceiptReqTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = MeteringReceiptReqTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = MeteringReceiptReqTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    outDoc.SessionID.bytesLen = MeteringReceiptReqTypeJson["SessionID"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.SessionID.bytesLen; i++) {
        outDoc.SessionID.bytes[i] = MeteringReceiptReqTypeJson["SessionID"]["bytes"][i].template get<uint8_t>();
    }
    if (MeteringReceiptReqTypeJson.contains("SAScheduleTupleID")) {
        outDoc.SAScheduleTupleID_isUsed = 1;
        outDoc.SAScheduleTupleID = MeteringReceiptReqTypeJson["SAScheduleTupleID"].template get<int16_t>();
    } else {
        outDoc.SAScheduleTupleID_isUsed = 0;
    }
    outDoc.MeterInfo = getDoc_MeterInfoType(MeteringReceiptReqTypeJson["MeterInfo"]);

    return outDoc;
}

json getJson_MeteringReceiptResType(const struct din_MeteringReceiptResType& MeteringReceiptResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = MeteringReceiptResTypeDoc.ResponseCode;
    outJson["AC_EVSEStatus"] = getJson_AC_EVSEStatusType(MeteringReceiptResTypeDoc.AC_EVSEStatus);

    return outJson;
}

struct din_MeteringReceiptResType getDoc_MeteringReceiptResType(const json& MeteringReceiptResTypeJson) {
    struct din_MeteringReceiptResType outDoc;
    init_din_MeteringReceiptResType(&outDoc);

    outDoc.ResponseCode = MeteringReceiptResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.AC_EVSEStatus = getDoc_AC_EVSEStatusType(MeteringReceiptResTypeJson["AC_EVSEStatus"]);

    return outDoc;
}

json getJson_SessionStopType(const struct din_SessionStopType& SessionStopTypeDoc) {
    json outJson;

    outJson["_unused"] = SessionStopTypeDoc._unused;
    return outJson;
}

struct din_SessionStopType getDoc_SessionStopType(const json& SessionStopTypeJson) {
    struct din_SessionStopType outDoc;
    init_din_SessionStopType(&outDoc);

    outDoc._unused = SessionStopTypeJson["_unused"].template get<int>();

    return outDoc;
}

json getJson_SessionStopResType(const struct din_SessionStopResType& SessionStopResTypeDoc) {
    json outJson;

    outJson["ResponseCode"] = SessionStopResTypeDoc.ResponseCode;

    return outJson;
}

struct din_SessionStopResType getDoc_SessionStopResType(const json& SessionStopResTypeJson) {
    struct din_SessionStopResType outDoc;
    init_din_SessionStopResType(&outDoc);

    outDoc.ResponseCode = SessionStopResTypeJson["ResponseCode"].template get<din_responseCodeType>();

    return outDoc;
}

json getJson_CertificateUpdateReqType(const struct din_CertificateUpdateReqType& CertificateUpdateReqTypeDoc) {
    json outJson;

    if (CertificateUpdateReqTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = CertificateUpdateReqTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < CertificateUpdateReqTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = CertificateUpdateReqTypeDoc.Id.characters[i];
        }
    }
    outJson["ContractSignatureCertChain"] = getJson_CertificateChainType(CertificateUpdateReqTypeDoc.ContractSignatureCertChain);
    outJson["ContractID"]["charactersLen"] = CertificateUpdateReqTypeDoc.ContractID.charactersLen;
    for (uint16_t i = 0; i < CertificateUpdateReqTypeDoc.ContractID.charactersLen; i++) {
        outJson["ContractID"]["characters"][i] = CertificateUpdateReqTypeDoc.ContractID.characters[i];
    }
    outJson["ListOfRootCertificateIDs"] = getJson_ListOfRootCertificateIDsType(CertificateUpdateReqTypeDoc.ListOfRootCertificateIDs);
    outJson["DHParams"]["bytesLen"] = CertificateUpdateReqTypeDoc.DHParams.bytesLen;
    for (uint16_t i = 0; i < CertificateUpdateReqTypeDoc.DHParams.bytesLen; i++) {
        outJson["DHParams"]["bytes"][i] = CertificateUpdateReqTypeDoc.DHParams.bytes[i];
    }

    return outJson;
}

struct din_CertificateUpdateReqType getDoc_CertificateUpdateReqType(const json& CertificateUpdateReqTypeJson) {
    struct din_CertificateUpdateReqType outDoc;
    init_din_CertificateUpdateReqType(&outDoc);

    if (CertificateUpdateReqTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = CertificateUpdateReqTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = CertificateUpdateReqTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    outDoc.ContractSignatureCertChain = getDoc_CertificateChainType(CertificateUpdateReqTypeJson["ContractSignatureCertChain"]);
    outDoc.ContractID.charactersLen = CertificateUpdateReqTypeJson["ContractID"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.ContractID.charactersLen; i++) {
        outDoc.ContractID.characters[i] = CertificateUpdateReqTypeJson["ContractID"]["characters"][i].template get<char>();
    }
    outDoc.ListOfRootCertificateIDs = getDoc_ListOfRootCertificateIDsType(CertificateUpdateReqTypeJson["ListOfRootCertificateIDs"]);
    outDoc.DHParams.bytesLen = CertificateUpdateReqTypeJson["DHParams"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.DHParams.bytesLen; i++) {
        outDoc.DHParams.bytes[i] = CertificateUpdateReqTypeJson["DHParams"]["bytes"][i].template get<uint8_t>();
    }

    return outDoc;
}

json getJson_CertificateUpdateResType(const struct din_CertificateUpdateResType& CertificateUpdateResTypeDoc) {
    json outJson;

    outJson["Id"]["charactersLen"] = CertificateUpdateResTypeDoc.Id.charactersLen;
    for (uint16_t i = 0; i < CertificateUpdateResTypeDoc.Id.charactersLen; i++) {
        outJson["Id"]["characters"][i] = CertificateUpdateResTypeDoc.Id.characters[i];
    }
    outJson["ResponseCode"] = CertificateUpdateResTypeDoc.ResponseCode;
    outJson["ContractSignatureCertChain"] = getJson_CertificateChainType(CertificateUpdateResTypeDoc.ContractSignatureCertChain);
    outJson["ContractSignatureEncryptedPrivateKey"]["bytesLen"] = CertificateUpdateResTypeDoc.ContractSignatureEncryptedPrivateKey.bytesLen;
    for (uint16_t i = 0; i < CertificateUpdateResTypeDoc.ContractSignatureEncryptedPrivateKey.bytesLen; i++) {
        outJson["ContractSignatureEncryptedPrivateKey"]["bytes"][i] = CertificateUpdateResTypeDoc.ContractSignatureEncryptedPrivateKey.bytes[i];
    }
    outJson["DHParams"]["bytesLen"] = CertificateUpdateResTypeDoc.DHParams.bytesLen;
    for (uint16_t i = 0; i < CertificateUpdateResTypeDoc.DHParams.bytesLen; i++) {
        outJson["DHParams"]["bytes"][i] = CertificateUpdateResTypeDoc.DHParams.bytes[i];
    }
    outJson["ContractID"]["charactersLen"] = CertificateUpdateResTypeDoc.ContractID.charactersLen;
    for (uint16_t i = 0; i < CertificateUpdateResTypeDoc.ContractID.charactersLen; i++) {
        outJson["ContractID"]["characters"][i] = CertificateUpdateResTypeDoc.ContractID.characters[i];
    }
    outJson["RetryCounter"] = CertificateUpdateResTypeDoc.RetryCounter;

    return outJson;
}

struct din_CertificateUpdateResType getDoc_CertificateUpdateResType(const json& CertificateUpdateResTypeJson) {
    struct din_CertificateUpdateResType outDoc;
    init_din_CertificateUpdateResType(&outDoc);

    outDoc.Id.charactersLen = CertificateUpdateResTypeJson["Id"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
        outDoc.Id.characters[i] = CertificateUpdateResTypeJson["Id"]["characters"][i].template get<char>();
    }
    outDoc.ResponseCode = CertificateUpdateResTypeJson["ResponseCode"].template get<din_responseCodeType>();
    outDoc.ContractSignatureCertChain = getDoc_CertificateChainType(CertificateUpdateResTypeJson["ContractSignatureCertChain"]);
    outDoc.ContractSignatureEncryptedPrivateKey.bytesLen = CertificateUpdateResTypeJson["ContractSignatureEncryptedPrivateKey"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.ContractSignatureEncryptedPrivateKey.bytesLen; i++) {
        outDoc.ContractSignatureEncryptedPrivateKey.bytes[i] = CertificateUpdateResTypeJson["ContractSignatureEncryptedPrivateKey"]["bytes"][i].template get<uint8_t>();
    }
    outDoc.DHParams.bytesLen = CertificateUpdateResTypeJson["DHParams"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.DHParams.bytesLen; i++) {
        outDoc.DHParams.bytes[i] = CertificateUpdateResTypeJson["DHParams"]["bytes"][i].template get<uint8_t>();
    }
    outDoc.ContractID.charactersLen = CertificateUpdateResTypeJson["ContractID"]["charactersLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.ContractID.charactersLen; i++) {
        outDoc.ContractID.characters[i] = CertificateUpdateResTypeJson["ContractID"]["characters"][i].template get<char>();
    }
    outDoc.RetryCounter = CertificateUpdateResTypeJson["RetryCounter"].template get<int16_t>();

    return outDoc;
}

json getJson_BodyBaseType(const struct din_BodyBaseType& BodyBaseTypeDoc) {
    json outJson;

    outJson["_unused"] = BodyBaseTypeDoc._unused;

    return outJson;
}

struct din_BodyBaseType getDoc_BodyBaseType(const json& BodyBaseTypeJson) {
    struct din_BodyBaseType outDoc;
    init_din_BodyBaseType(&outDoc);

    outDoc._unused = BodyBaseTypeJson["_unused"].template get<int>();

    return outDoc;
}

json getJson_NotificationType(const struct din_NotificationType& NotificationTypeDoc) {
    json outJson;

    outJson["FaultCode"] = NotificationTypeDoc.FaultCode;
    if (NotificationTypeDoc.FaultMsg_isUsed) {
        outJson["FaultMsg"]["charactersLen"] = NotificationTypeDoc.FaultMsg.charactersLen;
        for (uint16_t i = 0; i < NotificationTypeDoc.FaultMsg.charactersLen; i++) {
            outJson["FaultMsg"]["characters"][i] = NotificationTypeDoc.FaultMsg.characters[i];
        }
    }

    return outJson;
}

struct din_NotificationType getDoc_NotificationType(const json& NotificationTypeJson) {
    struct din_NotificationType outDoc;
    init_din_NotificationType(&outDoc);

    outDoc.FaultCode = NotificationTypeJson["FaultCode"].template get<din_faultCodeType>();
    if (NotificationTypeJson.contains("FaultMsg")) {
        outDoc.FaultMsg_isUsed = 1;
        outDoc.FaultMsg.charactersLen = NotificationTypeJson["FaultMsg"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.FaultMsg.charactersLen; i++) {
            outDoc.FaultMsg.characters[i] = NotificationTypeJson["FaultMsg"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.FaultMsg_isUsed = 0;
    }

    return outDoc;
}

json getJson_SignatureType(const struct din_SignatureType& SignatureTypeDoc) {
    json outJson;

    if (SignatureTypeDoc.Id_isUsed) {
        outJson["Id"]["charactersLen"] = SignatureTypeDoc.Id.charactersLen;
        for (uint16_t i = 0; i < SignatureTypeDoc.Id.charactersLen; i++) {
            outJson["Id"]["characters"][i] = SignatureTypeDoc.Id.characters[i];
        }
    }
    outJson["SignedInfo"] = getJson_SignedInfoType(SignatureTypeDoc.SignedInfo);
    outJson["SignatureValue"] = getJson_SignatureValueType(SignatureTypeDoc.SignatureValue);
    if (SignatureTypeDoc.KeyInfo_isUsed) {
        outJson["KeyInfo"] = getJson_KeyInfoType(SignatureTypeDoc.KeyInfo);
    }
    if (SignatureTypeDoc.Object_isUsed) {
        outJson["Object"] = getJson_ObjectType(SignatureTypeDoc.Object);
    }

    return outJson;
}

struct din_SignatureType getDoc_SignatureType(const json& SignatureTypeJson) {
    struct din_SignatureType outDoc;
    init_din_SignatureType(&outDoc);

    if (SignatureTypeJson.contains("Id")) {
        outDoc.Id_isUsed = 1;
        outDoc.Id.charactersLen = SignatureTypeJson["Id"]["charactersLen"].template get<uint16_t>();
        for (uint16_t i = 0; i < outDoc.Id.charactersLen; i++) {
            outDoc.Id.characters[i] = SignatureTypeJson["Id"]["characters"][i].template get<char>();
        }
    } else {
        outDoc.Id_isUsed = 0;
    }
    outDoc.SignedInfo = getDoc_SignedInfoType(SignatureTypeJson["SignedInfo"]);
    outDoc.SignatureValue = getDoc_SignatureValueType(SignatureTypeJson["SignatureValue"]);
    if (SignatureTypeJson.contains("KeyInfo")) {
        outDoc.KeyInfo_isUsed = 1;
        outDoc.KeyInfo = getDoc_KeyInfoType(SignatureTypeJson["KeyInfo"]);
    } else {
        outDoc.KeyInfo_isUsed = 0;
    }
    if (SignatureTypeJson.contains("Object")) {
        outDoc.Object_isUsed = 1;
        outDoc.Object = getDoc_ObjectType(SignatureTypeJson["Object"]);
    } else {
        outDoc.Object_isUsed = 0;
    }

    return outDoc;
}

json getJson_MessageHeaderType(const struct din_MessageHeaderType& MessageHeaderTypeDoc) {
    json outJson;

    outJson["SessionID"]["bytesLen"] = MessageHeaderTypeDoc.SessionID.bytesLen;
    for (uint16_t i = 0; i < MessageHeaderTypeDoc.SessionID.bytesLen; i++) {
        outJson["SessionID"]["bytes"][i] = MessageHeaderTypeDoc.SessionID.bytes[i];
    }
    if (MessageHeaderTypeDoc.Notification_isUsed) {
        outJson["Notification"] = getJson_NotificationType(MessageHeaderTypeDoc.Notification);
    }
    if (MessageHeaderTypeDoc.Signature_isUsed) {
        outJson["Signature"] = getJson_SignatureType(MessageHeaderTypeDoc.Signature);
    }

    return outJson;
}

struct din_MessageHeaderType getDoc_MessageHeaderType(const json& MessageHeaderTypeJson) {
    struct din_MessageHeaderType outDoc;
    init_din_MessageHeaderType(&outDoc);

    outDoc.SessionID.bytesLen = MessageHeaderTypeJson["SessionID"]["bytesLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.SessionID.bytesLen; i++) {
        outDoc.SessionID.bytes[i] = MessageHeaderTypeJson["SessionID"]["bytes"][i].template get<uint8_t>();
    }
    if (MessageHeaderTypeJson.contains("Notification")) {
        outDoc.Notification_isUsed = 1;
        outDoc.Notification = getDoc_NotificationType(MessageHeaderTypeJson["Notification"]);
    } else {
        outDoc.Notification_isUsed = 0;
    }
    if (MessageHeaderTypeJson.contains("Signature")) {
        outDoc.Signature_isUsed = 1;
        outDoc.Signature = getDoc_SignatureType(MessageHeaderTypeJson["Signature"]);
    } else {
        outDoc.Signature_isUsed = 0;
    }

    return outDoc;
}

json getJson_BodyType(const struct din_BodyType& BodyTypeDoc) {
    json outJson;

    if (BodyTypeDoc.BodyElement_isUsed) {
        outJson["BodyElement"] = getJson_BodyBaseType(BodyTypeDoc.BodyElement);
    }
    if (BodyTypeDoc.CableCheckReq_isUsed) {
        outJson["CableCheckReq"] = getJson_CableCheckReqType(BodyTypeDoc.CableCheckReq);
    }
    if (BodyTypeDoc.CableCheckRes_isUsed) {
        outJson["CableCheckRes"] = getJson_CableCheckResType(BodyTypeDoc.CableCheckRes);
    }
    if (BodyTypeDoc.CertificateInstallationReq_isUsed) {
        outJson["CertificateInstallationReq"] = getJson_CertificateInstallationReqType(BodyTypeDoc.CertificateInstallationReq);
    }
    if (BodyTypeDoc.CertificateInstallationRes_isUsed) {
        outJson["CertificateInstallationRes"] = getJson_CertificateInstallationResType(BodyTypeDoc.CertificateInstallationRes);
    }
    if (BodyTypeDoc.CertificateUpdateReq_isUsed) {
        outJson["CertificateUpdateReq"] = getJson_CertificateUpdateReqType(BodyTypeDoc.CertificateUpdateReq);
    }
    if (BodyTypeDoc.CertificateUpdateRes_isUsed) {
        outJson["CertificateUpdateRes"] = getJson_CertificateUpdateResType(BodyTypeDoc.CertificateUpdateRes);
    }
    if (BodyTypeDoc.ChargeParameterDiscoveryReq_isUsed) {
        outJson["ChargeParameterDiscoveryReq"] = getJson_ChargeParameterDiscoveryReqType(BodyTypeDoc.ChargeParameterDiscoveryReq);
    }
    if (BodyTypeDoc.ChargeParameterDiscoveryRes_isUsed) {
        outJson["ChargeParameterDiscoveryRes"] = getJson_ChargeParameterDiscoveryResType(BodyTypeDoc.ChargeParameterDiscoveryRes);
    }
    if (BodyTypeDoc.ChargingStatusReq_isUsed) {
        outJson["ChargingStatusReq"] = getJson_ChargingStatusReqType(BodyTypeDoc.ChargingStatusReq);
    }
    if (BodyTypeDoc.ChargingStatusRes_isUsed) {
        outJson["ChargingStatusRes"] = getJson_ChargingStatusResType(BodyTypeDoc.ChargingStatusRes);
    }
    if (BodyTypeDoc.ContractAuthenticationReq_isUsed) {
        outJson["ContractAuthenticationReq"] = getJson_ContractAuthenticationReqType(BodyTypeDoc.ContractAuthenticationReq);
    }
    if (BodyTypeDoc.ContractAuthenticationRes_isUsed) {
        outJson["ContractAuthenticationRes"] = getJson_ContractAuthenticationResType(BodyTypeDoc.ContractAuthenticationRes);
    }
    if (BodyTypeDoc.CurrentDemandReq_isUsed) {
        outJson["CurrentDemandReq"] = getJson_CurrentDemandReqType(BodyTypeDoc.CurrentDemandReq);
    }
    if (BodyTypeDoc.CurrentDemandRes_isUsed) {
        outJson["CurrentDemandRes"] = getJson_CurrentDemandResType(BodyTypeDoc.CurrentDemandRes);
    }
    if (BodyTypeDoc.MeteringReceiptReq_isUsed) {
        outJson["MeteringReceiptReq"] = getJson_MeteringReceiptReqType(BodyTypeDoc.MeteringReceiptReq);
    }
    if (BodyTypeDoc.MeteringReceiptRes_isUsed) {
        outJson["MeteringReceiptRes"] = getJson_MeteringReceiptResType(BodyTypeDoc.MeteringReceiptRes);
    }
    if (BodyTypeDoc.PaymentDetailsReq_isUsed) {
        outJson["PaymentDetailsReq"] = getJson_PaymentDetailsReqType(BodyTypeDoc.PaymentDetailsReq);
    }
    if (BodyTypeDoc.PaymentDetailsRes_isUsed) {
        outJson["PaymentDetailsRes"] = getJson_PaymentDetailsResType(BodyTypeDoc.PaymentDetailsRes);
    }
    if (BodyTypeDoc.PowerDeliveryReq_isUsed) {
        outJson["PowerDeliveryReq"] = getJson_PowerDeliveryReqType(BodyTypeDoc.PowerDeliveryReq);
    }
    if (BodyTypeDoc.PowerDeliveryRes_isUsed) {
        outJson["PowerDeliveryRes"] = getJson_PowerDeliveryResType(BodyTypeDoc.PowerDeliveryRes);
    }
    if (BodyTypeDoc.PreChargeReq_isUsed) {
        outJson["PreChargeReq"] = getJson_PreChargeReqType(BodyTypeDoc.PreChargeReq);
    }
    if (BodyTypeDoc.PreChargeRes_isUsed) {
        outJson["PreChargeRes"] = getJson_PreChargeResType(BodyTypeDoc.PreChargeRes);
    }
    if (BodyTypeDoc.ServiceDetailReq_isUsed) {
        outJson["ServiceDetailReq"] = getJson_ServiceDetailReqType(BodyTypeDoc.ServiceDetailReq);
    }
    if (BodyTypeDoc.ServiceDetailRes_isUsed) {
        outJson["ServiceDetailRes"] = getJson_ServiceDetailResType(BodyTypeDoc.ServiceDetailRes);
    }
    if (BodyTypeDoc.ServiceDiscoveryReq_isUsed) {
        outJson["ServiceDiscoveryReq"] = getJson_ServiceDiscoveryReqType(BodyTypeDoc.ServiceDiscoveryReq);
    }
    if (BodyTypeDoc.ServiceDiscoveryRes_isUsed) {
        outJson["ServiceDiscoveryRes"] = getJson_ServiceDiscoveryResType(BodyTypeDoc.ServiceDiscoveryRes);
    }
    if (BodyTypeDoc.ServicePaymentSelectionReq_isUsed) {
        outJson["ServicePaymentSelectionReq"] = getJson_ServicePaymentSelectionReqType(BodyTypeDoc.ServicePaymentSelectionReq);
    }
    if (BodyTypeDoc.ServicePaymentSelectionRes_isUsed) {
        outJson["ServicePaymentSelectionRes"] = getJson_ServicePaymentSelectionResType(BodyTypeDoc.ServicePaymentSelectionRes);
    }
    if (BodyTypeDoc.SessionSetupReq_isUsed) {
        outJson["SessionSetupReq"] = getJson_SessionSetupReqType(BodyTypeDoc.SessionSetupReq);
    }
    if (BodyTypeDoc.SessionSetupRes_isUsed) {
        outJson["SessionSetupRes"] = getJson_SessionSetupResType(BodyTypeDoc.SessionSetupRes);
    }
    if (BodyTypeDoc.SessionStopReq_isUsed) {
        outJson["SessionStopReq"] = getJson_SessionStopType(BodyTypeDoc.SessionStopReq);
    }
    if (BodyTypeDoc.SessionStopRes_isUsed) {
        outJson["SessionStopRes"] = getJson_SessionStopResType(BodyTypeDoc.SessionStopRes);
    }
    if (BodyTypeDoc.WeldingDetectionReq_isUsed) {
        outJson["WeldingDetectionReq"] = getJson_WeldingDetectionReqType(BodyTypeDoc.WeldingDetectionReq);
    }
    if (BodyTypeDoc.WeldingDetectionRes_isUsed) {
        outJson["WeldingDetectionRes"] = getJson_WeldingDetectionResType(BodyTypeDoc.WeldingDetectionRes);
    }

    return outJson;
}

struct din_BodyType getDoc_BodyType(const json& BodyTypeJson) {
    struct din_BodyType outDoc;
    init_din_BodyType(&outDoc);

    if (BodyTypeJson.contains("BodyElement")) {
        outDoc.BodyElement_isUsed = 1;
        outDoc.BodyElement = getDoc_BodyBaseType(BodyTypeJson["BodyElement"]);
    } else {
        outDoc.BodyElement_isUsed = 0;
    }
    if (BodyTypeJson.contains("CableCheckReq")) {
        outDoc.CableCheckReq_isUsed = 1;
        outDoc.CableCheckReq = getDoc_CableCheckReqType(BodyTypeJson["CableCheckReq"]);
    } else {
        outDoc.CableCheckReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("CableCheckRes")) {
        outDoc.CableCheckRes_isUsed = 1;
        outDoc.CableCheckRes = getDoc_CableCheckResType(BodyTypeJson["CableCheckRes"]);
    } else {
        outDoc.CableCheckRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("CertificateInstallationReq")) {
        outDoc.CertificateInstallationReq_isUsed = 1;
        outDoc.CertificateInstallationReq = getDoc_CertificateInstallationReqType(BodyTypeJson["CertificateInstallationReq"]);
    } else {
        outDoc.CertificateInstallationReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("CertificateInstallationRes")) {
        outDoc.CertificateInstallationRes_isUsed = 1;
        outDoc.CertificateInstallationRes = getDoc_CertificateInstallationResType(BodyTypeJson["CertificateInstallationRes"]);
    } else {
        outDoc.CertificateInstallationRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("CertificateUpdateReq")) {
        outDoc.CertificateUpdateReq_isUsed = 1;
        outDoc.CertificateUpdateReq = getDoc_CertificateUpdateReqType(BodyTypeJson["CertificateUpdateReq"]);
    } else {
        outDoc.CertificateUpdateReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("CertificateUpdateRes")) {
        outDoc.CertificateUpdateRes_isUsed = 1;
        outDoc.CertificateUpdateRes = getDoc_CertificateUpdateResType(BodyTypeJson["CertificateUpdateRes"]);
    } else {
        outDoc.CertificateUpdateRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("ChargeParameterDiscoveryReq")) {
        outDoc.ChargeParameterDiscoveryReq_isUsed = 1;
        outDoc.ChargeParameterDiscoveryReq = getDoc_ChargeParameterDiscoveryReqType(BodyTypeJson["ChargeParameterDiscoveryReq"]);
    } else {
        outDoc.ChargeParameterDiscoveryReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("ChargeParameterDiscoveryRes")) {
        outDoc.ChargeParameterDiscoveryRes_isUsed = 1;
        outDoc.ChargeParameterDiscoveryRes = getDoc_ChargeParameterDiscoveryResType(BodyTypeJson["ChargeParameterDiscoveryRes"]);
    } else {    
        outDoc.ChargeParameterDiscoveryRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("ChargingStatusReq")) {
        outDoc.ChargingStatusReq_isUsed = 1;
        outDoc.ChargingStatusReq = getDoc_ChargingStatusReqType(BodyTypeJson["ChargingStatusReq"]);
    } else {
        outDoc.ChargingStatusReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("ChargingStatusRes")) {
        outDoc.ChargingStatusRes_isUsed = 1;
        outDoc.ChargingStatusRes = getDoc_ChargingStatusResType(BodyTypeJson["ChargingStatusRes"]);
    } else {
        outDoc.ChargingStatusRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("ContractAuthenticationReq")) {
        outDoc.ContractAuthenticationReq_isUsed = 1;
        outDoc.ContractAuthenticationReq = getDoc_ContractAuthenticationReqType(BodyTypeJson["ContractAuthenticationReq"]);
    } else {
        outDoc.ContractAuthenticationReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("ContractAuthenticationRes")) {
        outDoc.ContractAuthenticationRes_isUsed = 1;
        outDoc.ContractAuthenticationRes = getDoc_ContractAuthenticationResType(BodyTypeJson["ContractAuthenticationRes"]);
    } else {
        outDoc.ContractAuthenticationRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("CurrentDemandReq")) {
        outDoc.CurrentDemandReq_isUsed = 1;
        outDoc.CurrentDemandReq = getDoc_CurrentDemandReqType(BodyTypeJson["CurrentDemandReq"]);
    } else {
        outDoc.CurrentDemandReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("CurrentDemandRes")) {
        outDoc.CurrentDemandRes_isUsed = 1;
        outDoc.CurrentDemandRes = getDoc_CurrentDemandResType(BodyTypeJson["CurrentDemandRes"]);
    } else {
        outDoc.CurrentDemandRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("MeteringReceiptReq")) {
        outDoc.MeteringReceiptReq_isUsed = 1;
        outDoc.MeteringReceiptReq = getDoc_MeteringReceiptReqType(BodyTypeJson["MeteringReceiptReq"]);
    } else {
        outDoc.MeteringReceiptReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("MeteringReceiptRes")) {
        outDoc.MeteringReceiptRes_isUsed = 1;
        outDoc.MeteringReceiptRes = getDoc_MeteringReceiptResType(BodyTypeJson["MeteringReceiptRes"]);
    } else {
        outDoc.MeteringReceiptRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("PaymentDetailsReq")) {
        outDoc.PaymentDetailsReq_isUsed = 1;
        outDoc.PaymentDetailsReq = getDoc_PaymentDetailsReqType(BodyTypeJson["PaymentDetailsReq"]);
    } else {
        outDoc.PaymentDetailsReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("PaymentDetailsRes")) {
        outDoc.PaymentDetailsRes_isUsed = 1;
        outDoc.PaymentDetailsRes = getDoc_PaymentDetailsResType(BodyTypeJson["PaymentDetailsRes"]);
    } else {
        outDoc.PaymentDetailsRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("PowerDeliveryReq")) {
        outDoc.PowerDeliveryReq_isUsed = 1;
        outDoc.PowerDeliveryReq = getDoc_PowerDeliveryReqType(BodyTypeJson["PowerDeliveryReq"]);
    } else {
        outDoc.PowerDeliveryReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("PowerDeliveryRes")) {
        outDoc.PowerDeliveryRes_isUsed = 1;
        outDoc.PowerDeliveryRes = getDoc_PowerDeliveryResType(BodyTypeJson["PowerDeliveryRes"]);
    } else {
        outDoc.PowerDeliveryRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("PreChargeReq")) {
        outDoc.PreChargeReq_isUsed = 1;
        outDoc.PreChargeReq = getDoc_PreChargeReqType(BodyTypeJson["PreChargeReq"]);
    } else {
        outDoc.PreChargeReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("PreChargeRes")) {
        outDoc.PreChargeRes_isUsed = 1;
        outDoc.PreChargeRes = getDoc_PreChargeResType(BodyTypeJson["PreChargeRes"]);
    } else {
        outDoc.PreChargeRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("ServiceDetailReq")) {
        outDoc.ServiceDetailReq_isUsed = 1;
        outDoc.ServiceDetailReq = getDoc_ServiceDetailReqType(BodyTypeJson["ServiceDetailReq"]);
    } else {
        outDoc.ServiceDetailReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("ServiceDetailRes")) {
        outDoc.ServiceDetailRes_isUsed = 1;
        outDoc.ServiceDetailRes = getDoc_ServiceDetailResType(BodyTypeJson["ServiceDetailRes"]);
    } else {
        outDoc.ServiceDetailRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("ServiceDiscoveryReq")) {
        outDoc.ServiceDiscoveryReq_isUsed = 1;
        outDoc.ServiceDiscoveryReq = getDoc_ServiceDiscoveryReqType(BodyTypeJson["ServiceDiscoveryReq"]);
    } else {
        outDoc.ServiceDiscoveryReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("ServiceDiscoveryRes")) {
        outDoc.ServiceDiscoveryRes_isUsed = 1;
        outDoc.ServiceDiscoveryRes = getDoc_ServiceDiscoveryResType(BodyTypeJson["ServiceDiscoveryRes"]);
    } else {
        outDoc.ServiceDiscoveryRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("ServicePaymentSelectionReq")) {
        outDoc.ServicePaymentSelectionReq_isUsed = 1;
        outDoc.ServicePaymentSelectionReq = getDoc_ServicePaymentSelectionReqType(BodyTypeJson["ServicePaymentSelectionReq"]);
    } else {
        outDoc.ServicePaymentSelectionReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("ServicePaymentSelectionRes")) {
        outDoc.ServicePaymentSelectionRes_isUsed = 1;
        outDoc.ServicePaymentSelectionRes = getDoc_ServicePaymentSelectionResType(BodyTypeJson["ServicePaymentSelectionRes"]);
    } else {
        outDoc.ServicePaymentSelectionRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("SessionSetupReq")) {
        outDoc.SessionSetupReq_isUsed = 1;
        outDoc.SessionSetupReq = getDoc_SessionSetupReqType(BodyTypeJson["SessionSetupReq"]);
    } else {
        outDoc.SessionSetupReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("SessionSetupRes")) {
        outDoc.SessionSetupRes_isUsed = 1;
        outDoc.SessionSetupRes = getDoc_SessionSetupResType(BodyTypeJson["SessionSetupRes"]);
    } else {
        outDoc.SessionSetupRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("SessionStopReq")) {
        outDoc.SessionStopReq_isUsed = 1;
        outDoc.SessionStopReq = getDoc_SessionStopType(BodyTypeJson["SessionStopReq"]);
    } else {
        outDoc.SessionStopReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("SessionStopRes")) {
        outDoc.SessionStopRes_isUsed = 1;
        outDoc.SessionStopRes = getDoc_SessionStopResType(BodyTypeJson["SessionStopRes"]);
    } else {
        outDoc.SessionStopRes_isUsed = 0;
    }
    if (BodyTypeJson.contains("WeldingDetectionReq")) {
        outDoc.WeldingDetectionReq_isUsed = 1;
        outDoc.WeldingDetectionReq = getDoc_WeldingDetectionReqType(BodyTypeJson["WeldingDetectionReq"]);
    } else {
        outDoc.WeldingDetectionReq_isUsed = 0;
    }
    if (BodyTypeJson.contains("WeldingDetectionRes")) {
        outDoc.WeldingDetectionRes_isUsed = 1;
        outDoc.WeldingDetectionRes = getDoc_WeldingDetectionResType(BodyTypeJson["WeldingDetectionRes"]);
    } else {
        outDoc.WeldingDetectionRes_isUsed = 0;
    }

    return outDoc;
}

json getJson_V2G_Message(const struct din_V2G_Message& V2G_MessageDoc) {
    json outJson;

    outJson["Header"] = getJson_MessageHeaderType(V2G_MessageDoc.Header);
    outJson["Body"] = getJson_BodyType(V2G_MessageDoc.Body);

    return outJson;
}

struct din_V2G_Message getDoc_V2G_Message(const json& V2G_MessageJson) {
    struct din_V2G_Message outDoc;
    init_din_V2G_Message(&outDoc);

    outDoc.Header = getDoc_MessageHeaderType(V2G_MessageJson["Header"]);
    outDoc.Body = getDoc_BodyType(V2G_MessageJson["Body"]);

    return outDoc;
}

json getJson_exiDocument(const struct din_exiDocument& exiDocumentDoc) {
    json outJson;

    outJson["V2G_Message"] = getJson_V2G_Message(exiDocumentDoc.V2G_Message);

    return outJson;
}

struct din_exiDocument getDoc_exiDocument(const json& exiDocumentJson) {
    struct din_exiDocument outDoc;
    init_din_exiDocument(&outDoc);

    outDoc.V2G_Message = getDoc_V2G_Message(exiDocumentJson["V2G_Message"]);

    return outDoc;
}

extern "C" {
    const char* decodeFromPythonBytes(const uint8_t* data, size_t size) {
        din_exiDocument outDoc;
        exi_bitstream_t inEXI;

        json outJson;

        exi_bitstream_init(&inEXI, const_cast<uint8_t*>(data), size, 0, nullptr);
        decode_din_exiDocument(&inEXI, &outDoc);

        outJson = getJson_exiDocument(outDoc);

        static string jsonString;
        jsonString = outJson.dump(4);

        return jsonString.c_str();
    }

    struct encoded_data {
        uint8_t* buffer;
        size_t size;
    };

    encoded_data* encodeFromPythonDict(const char* inString) {
        json inJson = json::parse(inString);

        din_exiDocument inDoc;
        inDoc = getDoc_exiDocument(inJson);

        uint8_t* stream = new uint8_t[256];
        exi_bitstream_t outEXI;
        size_t pos1 = 0;

        exi_bitstream_init(&outEXI, stream, 256, pos1, nullptr);
        encode_din_exiDocument(&outEXI, &inDoc);
        
        encoded_data* result = new encoded_data;
        result->size = exi_bitstream_get_length(&outEXI);
        result->buffer = new uint8_t[result->size];
        memcpy(result->buffer, stream, result->size);
        
        delete[] stream;
        return result;
    }

    void free_encoded_data(encoded_data* data) {
        if (data) {
            delete[] data->buffer;
            delete data;
        }
    }
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("./DINProcessor --[encode/decode]='STRING_OPTIONAL' -i FILE_INPUT -o FILE_OUTPUT\n  ./DINProcessor --decode='809a021050908c0c0c0c0c51e000040080'\n  ./DINProcessor -e -i input.json -o output.txt\n  ./DINProcessor", "Encode and decode DIN EXI messages\nIf no arguement specified for input/outfile, will default to stdin/stdout\nMUST use double quotes (\") and lower case true/false");
    options.add_options()
        ("h,help", "Show help")
        ("e,encode", "Encode EXI message from JSON to EXI bytes", cxxopts::value<std::string>()->implicit_value("cmd"))
        ("d,decode", "Decode EXI message from EXI bytes to JSON", cxxopts::value<std::string>()->implicit_value("cmd"))
        ("i,input", "Input file to read JSON or EXI bytes from", cxxopts::value<std::string>())
        ("o,output", "Output file to write JSON or EXI bytes to", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    // Error handling
    if (result.count("encode") == 0 && result.count("decode") == 0) {
        std::cerr << "Error: Must specify either encode or decode option." << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    if (result.count("encode") && result.count("decode")) {
        std::cerr << "Error: Cannot specify both encode and decode options." << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    if (result.count("encode")) {
        if (result["encode"].as<std::string>() != "cmd" && result.count("input")) {
            std::cerr << "Error: Cannot encode string input with file input option." << std::endl;
            std::cout << options.help() << std::endl;
            return 1;
        }

        string inString;
        if (result.count("input")) {
            ifstream inputFile(result["input"].as<std::string>());
            if (inputFile.is_open()) {
                stringstream buffer;
                buffer << inputFile.rdbuf();
                inString = buffer.str();
                inputFile.close();
            } else {
                std::cerr << "Error: Could not open input file." << std::endl;
                return 1;
            }
        
        } else {
            inString = result["encode"].as<std::string>();
        }

        // cout << "Input JSON string: " << inString << endl;

        bool isValid = json::accept(inString);
        if (!isValid) {
            std::cerr << "Error: Invalid JSON input." << std::endl;
            return 1;
        }

        encoded_data* encodedResult = encodeFromPythonDict(inString.c_str());
        if (result.count("output")) {
            ofstream outputFile(result["output"].as<std::string>());
            if (outputFile.is_open()) {
                // Convert bytes to hex string
                stringstream hexStream;
                for (size_t i = 0; i < encodedResult->size; ++i) {
                    hexStream << std::hex << std::setw(2) << std::setfill('0') << (int)encodedResult->buffer[i];
                }
                outputFile << hexStream.str();
                outputFile.close();
            } else {
                std::cerr << "Error: Could not open output file." << std::endl;
                free_encoded_data(encodedResult);
                return 1;
            }
        } else {
            for (size_t i = 0; i < encodedResult->size; ++i) {
                cout << std::hex << std::setw(2) << std::setfill('0') << (int)encodedResult->buffer[i];
            }
            cout << endl;
        }

    }

    if (result.count("decode")) {
        if (result["decode"].as<std::string>() != "cmd" && result.count("input")) {
            std::cerr << "Error: Cannot decode string input with file input option." << std::endl;
            std::cout << options.help() << std::endl;
            return 1;
        }

        string hexString;
        if (result.count("input")) {
            ifstream inputFile(result["input"].as<std::string>());
            if (inputFile.is_open()) {
                stringstream buffer;
                buffer << inputFile.rdbuf();
                hexString = buffer.str();
                inputFile.close();
            } else {
                std::cerr << "Error: Could not open input file." << std::endl;
                return 1;
            }
        } else {
            hexString = result["decode"].as<std::string>();
        }

        // Convert hex string to bytes
        if (hexString.length() % 2 != 0) {
            std::cerr << "Error: Invalid hex string length." << std::endl;
            return 1;
        }

        vector<uint8_t> bytes;
        for (size_t i = 0; i < hexString.length(); i += 2) {
            string byteString = hexString.substr(i, 2);
            try {
                uint8_t byte = static_cast<uint8_t>(stoi(byteString, nullptr, 16));
                bytes.push_back(byte);
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid hex string format." << std::endl;
                return 1;
            }
        }

        const char* jsonString = decodeFromPythonBytes(bytes.data(), bytes.size());
        if (result.count("output")) {
            ofstream outputFile(result["output"].as<std::string>());
            if (outputFile.is_open()) {
                // Try to parse and pretty print JSON
                try {
                    json j = json::parse(jsonString);
                    outputFile << j.dump(4);
                    outputFile.close();
                } catch (const json::parse_error& e) {
                    std::cerr << "Error: Failed to parse JSON output." << std::endl;
                    return 1;
                }
            } else {
                std::cerr << "Error: Could not open output file." << std::endl;
                return 1;
            }
        } else {
            try {
                json j = json::parse(jsonString);
                cout << j.dump(4) << endl;
            } catch (const json::parse_error& e) {
                std::cerr << "Error: Failed to parse JSON output." << std::endl;
                return 1;
            }
        }
    }
    return 0;
}