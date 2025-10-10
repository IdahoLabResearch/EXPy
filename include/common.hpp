/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

#ifndef COMMON_HPP
#define COMMON_HPP

#include <cbv2g/common/exi_basetypes.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

json getJson_exi_binary_t(const exi_binary_t& exiBinary);
exi_binary_t getDoc_exi_binary_t(const json& exiBinaryJson);

json getJson_exi_unsigned_t(const exi_unsigned_t& exiUnsigned);
exi_unsigned_t getDoc_exi_unsigned_t(const json& exiUnsignedJson);

json getJson_exi_signed_t(const exi_signed_t& exiSigned);
exi_signed_t getDoc_exi_signed_t(const json& exiSignedJson);

#endif // COMMON_HPP