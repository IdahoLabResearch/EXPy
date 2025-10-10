#ifndef APP_HANDSHAKE_PROCESSOR_HPP
#define APP_HANDSHAKE_PROCESSOR_HPP

#include <nlohmann/json.hpp>
using json = nlohmann::json;

json getJson_AppProtocolType(const struct appHand_AppProtocolType& appProtocolDoc);
struct appHand_AppProtocolType getDoc_AppProtocolType(const json& appProtocolJson);

json getJson_supportedAppProtocolReq(const struct appHand_supportedAppProtocolReq& supportedAppProtocolReqDoc);
struct appHand_supportedAppProtocolReq getDoc_supportedAppProtocolReq(const json& supportedAppProtocolReqJson);

json getJson_supportedAppProtocolRes(const struct appHand_supportedAppProtocolRes& supportedAppProtocolResDoc);
struct appHand_supportedAppProtocolRes getDoc_supportedAppProtocolRes(const json& supportedAppProtocolResJson);

json getJson_exiDocument(const struct appHand_exiDocument& exiDoc);
struct appHand_exiDocument getDoc_exiDocument(const json& exiJson);

struct encoded_data {
    uint8_t* buffer;
    size_t size;
};

const char* decodeFromPythonBytes(const uint8_t* data, size_t size);
encoded_data* encodeFromPythonDict(const char* inString);
void free_encoded_data(encoded_data* data);

#endif // APP_HANDSHAKE_PROCESSOR_HPP