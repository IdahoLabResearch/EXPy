/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

#ifndef COMMON_HPP
#define COMMON_HPP

#include <cbv2g/common/exi_basetypes.h>

// nlohmann::json uses JSON_ASSERT (default: `assert`) on missing-key const
// operator[] and similar precondition checks. With a release-style assert that
// aborts, the C++ entry point would tear the process down before our outer
// try/catch can convert the failure into an EXPY_ERROR__MARSHALER_INPUT rc. We
// override the macro to throw, so the consumer-reachable checks (const op[],
// typed accessors) become catchable exceptions instead.
// MUST appear before <nlohmann/json.hpp> is included anywhere.
//
// -Wterminate fires because nlohmann also invokes JSON_ASSERT from a handful of
// internal noexcept methods (mostly iterator helpers); a throw there would call
// std::terminate. None of those are reachable from EXPy's encode/decode entry
// points — they're guarded by upstream is_object / is_array checks — and even
// if one ever were, std::terminate is still strictly better than the silent
// abort we had before #21. The warning would otherwise spam every TU.
#include <stdexcept>
#define JSON_ASSERT(x) do { if (!(x)) { throw std::runtime_error("nlohmann::json assertion failed: " #x); } } while (0)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wterminate"
#include <nlohmann/json.hpp>
#pragma GCC diagnostic pop
using json = nlohmann::json;

// Sentinel status surfaced through encoded_data.status / decoded_data.status when
// the failure originated in the C++ JSON layer (nlohmann::json::parse, marshaler
// const-operator[] on a missing key, template get<T>() on a wrong-typed value)
// rather than in libcbv2g's encoder/decoder. Chosen well outside libcbv2g's range
// (currently -1 .. -299) so the two surfaces are distinguishable. See ADR-0006.
#define EXPY_ERROR__MARSHALER_INPUT -1000

json getJson_exi_binary_t(const exi_binary_t& exiBinary);
exi_binary_t getDoc_exi_binary_t(const json& exiBinaryJson);

json getJson_exi_unsigned_t(const exi_unsigned_t& exiUnsigned);
exi_unsigned_t getDoc_exi_unsigned_t(const json& exiUnsignedJson);

json getJson_exi_signed_t(const exi_signed_t& exiSigned);
exi_signed_t getDoc_exi_signed_t(const json& exiSignedJson);

#endif // COMMON_HPP