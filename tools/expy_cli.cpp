/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

// Namespace-agnostic CLI for any per-Namespace Processor object. Linking this
// translation unit alongside <Namespace>Processor.generated.o produces an
// executable that encodes/decodes that Namespace's Documents from JSON/EXI on
// stdio or filesystem paths.
//
// The CLI talks only to the four extern "C" symbols every Processor exposes
// (decode/encode Document + free_*). It does not include any per-Namespace
// header, so it stays usable as a single shared source for SAP/DIN/ISO2.

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

extern "C" {
    struct decoded_data {
        const char* json;
        int status;
    };

    struct encoded_data {
        uint8_t* buffer;
        size_t size;
        int status;
    };

    decoded_data* decodeFromPythonBytes(const uint8_t* data, size_t size);
    encoded_data* encodeFromPythonDict(const char* inString);
    void free_encoded_data(encoded_data* data);
    void free_decoded_data(decoded_data* data);
}


int main(int argc, char* argv[]) {
    cxxopts::Options options(
        "./EXIProcessor --[encode/decode]='STRING_OPTIONAL' -i FILE_INPUT -o FILE_OUTPUT",
        "Encode and decode V2G EXI Documents for one Namespace.\n"
        "If no argument is given for input/output, defaults to stdin/stdout.\n"
        "Use double quotes (\") and lower case true/false."
    );
    options.add_options()
        ("h,help", "Show help")
        ("e,encode", "Encode EXI Document from JSON to EXI bytes",
            cxxopts::value<std::string>()->implicit_value("cmd"))
        ("d,decode", "Decode EXI Document from EXI bytes to JSON",
            cxxopts::value<std::string>()->implicit_value("cmd"))
        ("i,input", "Input file (JSON for encode, hex EXI for decode)",
            cxxopts::value<std::string>())
        ("o,output", "Output file",
            cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

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
            if (!inputFile.is_open()) {
                std::cerr << "Error: Could not open input file." << std::endl;
                return 1;
            }
            stringstream buffer;
            buffer << inputFile.rdbuf();
            inString = buffer.str();
        } else {
            inString = result["encode"].as<std::string>();
        }

        if (!json::accept(inString)) {
            std::cerr << "Error: Invalid JSON input." << std::endl;
            return 1;
        }

        encoded_data* encodedResult = encodeFromPythonDict(inString.c_str());
        stringstream hexStream;
        for (size_t i = 0; i < encodedResult->size; ++i) {
            hexStream << std::hex << std::setw(2) << std::setfill('0')
                      << (int)encodedResult->buffer[i];
        }
        if (result.count("output")) {
            ofstream outputFile(result["output"].as<std::string>());
            if (!outputFile.is_open()) {
                std::cerr << "Error: Could not open output file." << std::endl;
                free_encoded_data(encodedResult);
                return 1;
            }
            outputFile << hexStream.str();
        } else {
            cout << hexStream.str() << endl;
        }
        free_encoded_data(encodedResult);
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
            if (!inputFile.is_open()) {
                std::cerr << "Error: Could not open input file." << std::endl;
                return 1;
            }
            stringstream buffer;
            buffer << inputFile.rdbuf();
            hexString = buffer.str();
        } else {
            hexString = result["decode"].as<std::string>();
        }

        if (hexString.length() % 2 != 0) {
            std::cerr << "Error: Invalid hex string length." << std::endl;
            return 1;
        }

        vector<uint8_t> bytes;
        for (size_t i = 0; i < hexString.length(); i += 2) {
            try {
                uint8_t byte = static_cast<uint8_t>(stoi(hexString.substr(i, 2), nullptr, 16));
                bytes.push_back(byte);
            } catch (const std::exception&) {
                std::cerr << "Error: Invalid hex string format." << std::endl;
                return 1;
            }
        }

        decoded_data* decoded = decodeFromPythonBytes(bytes.data(), bytes.size());
        if (decoded->status != 0) {
            std::cerr << "Error: libcbv2g decode failed (rc=" << decoded->status << ")." << std::endl;
            free_decoded_data(decoded);
            return 1;
        }

        try {
            json j = json::parse(decoded->json);
            if (result.count("output")) {
                ofstream outputFile(result["output"].as<std::string>());
                if (!outputFile.is_open()) {
                    std::cerr << "Error: Could not open output file." << std::endl;
                    free_decoded_data(decoded);
                    return 1;
                }
                outputFile << j.dump(4);
            } else {
                cout << j.dump(4) << endl;
            }
        } catch (const json::parse_error&) {
            std::cerr << "Error: Failed to parse JSON output." << std::endl;
            free_decoded_data(decoded);
            return 1;
        }
        free_decoded_data(decoded);
    }
    return 0;
}
