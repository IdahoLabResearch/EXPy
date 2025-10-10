/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

#include <cbv2g/common/exi_basetypes.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

json getJson_exi_binary_t(const exi_binary_t& exiBinary) {
    json outJson;

    outJson["octets_size"] = exiBinary.octets_size;
    outJson["octets_count"] = exiBinary.octets_count;
    for (size_t i = 0; i < exiBinary.octets_size; ++i) {
        outJson["octets"][i] = exiBinary.octets[i];
    }
    return outJson;
}

exi_binary_t getDoc_exi_binary_t(const json& exiBinaryJson) {
    exi_binary_t outDoc;

    outDoc.octets_size = exiBinaryJson["octets_size"].template get<size_t>();
    outDoc.octets_count = exiBinaryJson["octets_count"].template get<size_t>();
    outDoc.octets = new uint8_t[outDoc.octets_size];
    for (size_t i = 0; i < outDoc.octets_size; ++i) {
        outDoc.octets[i] = exiBinaryJson["octets"][i].template get<uint8_t>();
    }
    return outDoc;
}

json getJson_exi_unsigned_t(const exi_unsigned_t& exiUnsigned) {
    json outJson;

    outJson["octets_count"] = exiUnsigned.octets_count;
    for (size_t i = 0; i < exiUnsigned.octets_count; ++i) {
        outJson["octets"][i] = exiUnsigned.octets[i];
    }
    return outJson;
}

exi_unsigned_t getDoc_exi_unsigned_t(const json& exiUnsignedJson) {
    exi_unsigned_t outDoc;

    outDoc.octets_count = exiUnsignedJson["octets_count"].template get<size_t>();
    for (size_t i = 0; i < outDoc.octets_count; ++i) {
        outDoc.octets[i] = exiUnsignedJson["octets"][i].template get<uint8_t>();
    }
    return outDoc;
}

json getJson_exi_signed_t(const exi_signed_t& exiSigned) {
    json outJson;

    outJson["data"] = getJson_exi_unsigned_t(exiSigned.data);
    outJson["is_negative"] = exiSigned.is_negative & 0x1;
    return outJson;

}

exi_signed_t getDoc_exi_signed_t(const json& exiSignedJson) {
    exi_signed_t outDoc;

    outDoc.data = getDoc_exi_unsigned_t(exiSignedJson["data"]);
    outDoc.is_negative = exiSignedJson["is_negative"].template get<uint8_t>() & 0x1;
    return outDoc;
}