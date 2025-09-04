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

extern "C" {
    const char* decodeFromPythonBytes(const uint8_t* data, size_t size) {
        appHand_exiDocument outDoc;
        exi_bitstream_t inEXI;

        json outJson;

        exi_bitstream_init(&inEXI, const_cast<uint8_t*>(data), size, 0, nullptr);
        decode_appHand_exiDocument(&inEXI, &outDoc);

        if (outDoc.supportedAppProtocolReq_isUsed) {
            for (int i = 0; i < outDoc.supportedAppProtocolReq.AppProtocol.arrayLen; ++i) {
                outJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["ProtocolNamespace"]["charactersLen"] =
                    outDoc.supportedAppProtocolReq.AppProtocol.array[i].ProtocolNamespace.charactersLen;
                for (int j = 0; j < outDoc.supportedAppProtocolReq.AppProtocol.array[i].ProtocolNamespace.charactersLen; ++j) {
                    outJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["ProtocolNamespace"]["characters"][j] =
                        outDoc.supportedAppProtocolReq.AppProtocol.array[i].ProtocolNamespace.characters[j];
                }
                outJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["VersionNumberMajor"] =
                    outDoc.supportedAppProtocolReq.AppProtocol.array[i].VersionNumberMajor;
                outJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["VersionNumberMinor"] =
                    outDoc.supportedAppProtocolReq.AppProtocol.array[i].VersionNumberMinor;
                outJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["SchemaID"] =
                    outDoc.supportedAppProtocolReq.AppProtocol.array[i].SchemaID;
                outJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["Priority"] =
                    outDoc.supportedAppProtocolReq.AppProtocol.array[i].Priority;
            }
        }

        if (outDoc.supportedAppProtocolRes_isUsed) {
            outJson["supportedAppProtocolRes"]["ResponseCode"] = outDoc.supportedAppProtocolRes.ResponseCode;
            if (outDoc.supportedAppProtocolRes.SchemaID_isUsed) {
                outJson["supportedAppProtocolRes"]["SchemaID"] = outDoc.supportedAppProtocolRes.SchemaID;
            }
        }

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

        appHand_exiDocument inDoc;
        init_appHand_exiDocument(&inDoc);

        if (inJson.contains("supportedAppProtocolReq")) {
            inDoc.supportedAppProtocolReq_isUsed = 1;

            inDoc.supportedAppProtocolReq.AppProtocol.arrayLen = inJson["supportedAppProtocolReq"]["AppProtocol"]["array"].size();
            for (uint16_t i = 0; i < inDoc.supportedAppProtocolReq.AppProtocol.arrayLen; ++i) {
                inDoc.supportedAppProtocolReq.AppProtocol.array[i].ProtocolNamespace.charactersLen =
                    inJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["ProtocolNamespace"]["characters"].size();
                for (uint16_t j = 0; j < inDoc.supportedAppProtocolReq.AppProtocol.array[i].ProtocolNamespace.charactersLen; ++j) {
                    inDoc.supportedAppProtocolReq.AppProtocol.array[i].ProtocolNamespace.characters[j] =
                        inJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["ProtocolNamespace"]["characters"][j].template get<char>();
                }
                inDoc.supportedAppProtocolReq.AppProtocol.array[i].VersionNumberMajor =
                    inJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["VersionNumberMajor"].template get<uint32_t>();
                inDoc.supportedAppProtocolReq.AppProtocol.array[i].VersionNumberMinor =
                    inJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["VersionNumberMinor"].template get<uint32_t>();
                inDoc.supportedAppProtocolReq.AppProtocol.array[i].SchemaID =
                    inJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["SchemaID"].template get<uint8_t>();
                inDoc.supportedAppProtocolReq.AppProtocol.array[i].Priority =
                    inJson["supportedAppProtocolReq"]["AppProtocol"]["array"][i]["Priority"].template get<uint8_t>();
            }
        } else if (inJson.contains("supportedAppProtocolRes")) {
            inDoc.supportedAppProtocolRes_isUsed = 1;

            inDoc.supportedAppProtocolRes.ResponseCode =
                inJson["supportedAppProtocolRes"]["ResponseCode"].template get<appHand_responseCodeType>();
            if (inJson["supportedAppProtocolRes"].contains("SchemaID")) {
                inDoc.supportedAppProtocolRes.SchemaID_isUsed = 1;
                inDoc.supportedAppProtocolRes.SchemaID =
                    inJson["supportedAppProtocolRes"]["SchemaID"].template get<uint8_t>();
            } else {
                inDoc.supportedAppProtocolRes.SchemaID_isUsed = 0;
            }
        }

        uint8_t* stream = new uint8_t[256];  // Dynamically allocate buffer
        exi_bitstream_t outEXI;
        size_t pos1 = 0;

        exi_bitstream_init(&outEXI, stream, 256, pos1, nullptr);
        encode_appHand_exiDocument(&outEXI, &inDoc);
        
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
    cxxopts::Options options("./AppHandshakeProcessor --[encode/decode]='STRING_OPTIONAL' -i FILE_INPUT -o FILE_OUTPUT\n  ./AppHandshakeProcessor --decode='80400040'\n  ./AppHandshakeProcessor -e -i input.json -o output.txt\n  ./AppHandshakeProcessor", "Encode and decode AppHandshake EXI messages\nIf no arguement specified for input/outfile, will default to stdin/stdout\nMUST use double quotes (\") and lower case true/false");
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