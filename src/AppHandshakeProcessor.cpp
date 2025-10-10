/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <new>

#include <cbv2g/app_handshake/appHand_Decoder.h>
#include <cbv2g/app_handshake/appHand_Encoder.h>
#include <cbv2g/app_handshake/appHand_Datatypes.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <cxxopts.hpp>

#include <iostream>
#include <fstream>
using namespace std;

json getJson_AppProtocolType(const struct appHand_AppProtocolType& appProtocolDoc) {
    json outJson;

    outJson["ProtocolNamespace"]["charactersLen"] = appProtocolDoc.ProtocolNamespace.charactersLen;
    for (uint16_t j = 0; j < appProtocolDoc.ProtocolNamespace.charactersLen; ++j) {
        outJson["ProtocolNamespace"]["characters"][j] = appProtocolDoc.ProtocolNamespace.characters[j];
    }
    outJson["VersionNumberMajor"] = appProtocolDoc.VersionNumberMajor;
    outJson["VersionNumberMinor"] = appProtocolDoc.VersionNumberMinor;
    outJson["SchemaID"] = appProtocolDoc.SchemaID;
    outJson["Priority"] = appProtocolDoc.Priority;
    return outJson;
}

struct appHand_AppProtocolType getDoc_AppProtocolType(const json& appProtocolJson) {
    struct appHand_AppProtocolType outDoc;
    init_appHand_AppProtocolType(&outDoc);

    outDoc.ProtocolNamespace.charactersLen = appProtocolJson["ProtocolNamespace"]["charactersLen"].template get<uint16_t>();
    for (uint16_t j = 0; j < outDoc.ProtocolNamespace.charactersLen; ++j) {
        outDoc.ProtocolNamespace.characters[j] = appProtocolJson["ProtocolNamespace"]["characters"][j].template get<char>();
    }
    outDoc.VersionNumberMajor = appProtocolJson["VersionNumberMajor"].template get<uint32_t>();
    outDoc.VersionNumberMinor = appProtocolJson["VersionNumberMinor"].template get<uint32_t>();
    outDoc.SchemaID = appProtocolJson["SchemaID"].template get<uint8_t>();
    outDoc.Priority = appProtocolJson["Priority"].template get<uint8_t>();
    return outDoc;
} 

json getJson_supportedAppProtocolReq(const struct appHand_supportedAppProtocolReq& supportedAppProtocolReqDoc) {
    json outJson;

    outJson["AppProtocol"]["arrayLen"] = supportedAppProtocolReqDoc.AppProtocol.arrayLen;
    for (uint16_t i = 0; i < supportedAppProtocolReqDoc.AppProtocol.arrayLen; ++i) {
        outJson["AppProtocol"]["array"][i] = getJson_AppProtocolType(supportedAppProtocolReqDoc.AppProtocol.array[i]);
    }
    return outJson;
}

struct appHand_supportedAppProtocolReq getDoc_supportedAppProtocolReq(const json& supportedAppProtocolReqJson) {
    struct appHand_supportedAppProtocolReq outDoc;
    init_appHand_supportedAppProtocolReq(&outDoc);

    outDoc.AppProtocol.arrayLen = supportedAppProtocolReqJson["AppProtocol"]["arrayLen"].template get<uint16_t>();
    for (uint16_t i = 0; i < outDoc.AppProtocol.arrayLen; ++i) {
        outDoc.AppProtocol.array[i] = getDoc_AppProtocolType(supportedAppProtocolReqJson["AppProtocol"]["array"][i]);
    }
    return outDoc;
}

json getJson_supportedAppProtocolRes(const struct appHand_supportedAppProtocolRes& supportedAppProtocolResDoc) {
    json outJson;

    outJson["ResponseCode"] = supportedAppProtocolResDoc.ResponseCode;
    if (supportedAppProtocolResDoc.SchemaID_isUsed) {
        outJson["SchemaID"] = supportedAppProtocolResDoc.SchemaID;
    }
    return outJson;
}

struct appHand_supportedAppProtocolRes getDoc_supportedAppProtocolRes(const json& supportedAppProtocolResJson) {
    struct appHand_supportedAppProtocolRes outDoc;
    init_appHand_supportedAppProtocolRes(&outDoc);

    outDoc.ResponseCode = supportedAppProtocolResJson["ResponseCode"].template get<appHand_responseCodeType>();
    if (supportedAppProtocolResJson.contains("SchemaID")) {
        outDoc.SchemaID_isUsed = 1;
        outDoc.SchemaID = supportedAppProtocolResJson["SchemaID"].template get<uint8_t>();
    } else {
        outDoc.SchemaID_isUsed = 0;
    }
    return outDoc;
}

json getJson_exiDocument(const struct appHand_exiDocument& exiDoc) {
    json outJson;

    if (exiDoc.supportedAppProtocolReq_isUsed) {
        outJson["supportedAppProtocolReq"] = getJson_supportedAppProtocolReq(exiDoc.supportedAppProtocolReq);
    }
    if (exiDoc.supportedAppProtocolRes_isUsed) {
        outJson["supportedAppProtocolRes"] = getJson_supportedAppProtocolRes(exiDoc.supportedAppProtocolRes);
    }
    return outJson;
}

struct appHand_exiDocument getDoc_exiDocument(const json& exiJson) {
    struct appHand_exiDocument outDoc;
    init_appHand_exiDocument(&outDoc);

    if (exiJson.contains("supportedAppProtocolReq")) {
        outDoc.supportedAppProtocolReq_isUsed = 1;
        outDoc.supportedAppProtocolReq = getDoc_supportedAppProtocolReq(exiJson["supportedAppProtocolReq"]);
    } else if (exiJson.contains("supportedAppProtocolRes")) {
        outDoc.supportedAppProtocolRes_isUsed = 1;
        outDoc.supportedAppProtocolRes = getDoc_supportedAppProtocolRes(exiJson["supportedAppProtocolRes"]);
    }
    return outDoc;
}

extern "C" {
    const char* decodeFromPythonBytes(const uint8_t* data, size_t size) {
        appHand_exiDocument outDoc;
        exi_bitstream_t inEXI;

        json outJson;

        exi_bitstream_init(&inEXI, const_cast<uint8_t*>(data), size, 0, nullptr);
        decode_appHand_exiDocument(&inEXI, &outDoc);

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

        struct appHand_exiDocument outDoc;
        init_appHand_exiDocument(&outDoc);

        outDoc = getDoc_exiDocument(inJson);

        uint8_t* stream = new uint8_t[256];
        exi_bitstream_t outEXI;
        size_t pos1 = 0;

        exi_bitstream_init(&outEXI, stream, 256, pos1, nullptr);
        encode_appHand_exiDocument(&outEXI, &outDoc);

        cout << inJson.dump(4) << endl;
        
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
    cxxopts::Options options("./AppHandshakeProcessor --[encode/decode]='STRING_OPTIONAL' -i FILE_INPUT -o FILE_OUTPUT\n  ./AppHandshakeProcessor --decode='80400040'\n  ./AppHandshakeProcessor -e -i input.json -o output.txt\n  ./AppHandshakeProcessor", "Encode and decode AppHandshake EXI messages\nIf no argument specified for input/outfile, will default to stdin/stdout\nMUST use double quotes (\") and lower case true/false");
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