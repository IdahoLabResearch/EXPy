/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

// Shared CLI helpers for the per-Namespace CLI sources emitted by
// tools/codegen/processor_cli_emitter.py. Header-only so each generated TU
// instantiates its own copies; both helpers take Processor entry-point and
// matching free function as parameters so the same body drives Document /
// Fragment / XmldsigFragment encode/decode for any Namespace.
//
// The includer must `#include "<Namespace>Processor.generated.hpp"` BEFORE
// this header so `encoded_data` / `decoded_data` are declared.

#pragma once

#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

inline int expy_cli_encode(
    encoded_data* (*encode_fn)(const char*),
    void (*free_fn)(encoded_data*),
    const cxxopts::ParseResult& result,
    const std::string& flag_value
) {
    using json = nlohmann::json;

    if (flag_value != "cmd" && result.count("input")) {
        std::cerr << "Error: Cannot encode string input with file input option." << std::endl;
        return 1;
    }

    std::string inString;
    if (result.count("input")) {
        std::ifstream inputFile(result["input"].as<std::string>());
        if (!inputFile.is_open()) {
            std::cerr << "Error: Could not open input file." << std::endl;
            return 1;
        }
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        inString = buffer.str();
    } else {
        inString = flag_value;
    }

    if (!json::accept(inString)) {
        std::cerr << "Error: Invalid JSON input." << std::endl;
        return 1;
    }

    encoded_data* encodedResult = encode_fn(inString.c_str());
    std::stringstream hexStream;
    for (size_t i = 0; i < encodedResult->size; ++i) {
        hexStream << std::hex << std::setw(2) << std::setfill('0')
                  << (int)encodedResult->buffer[i];
    }
    if (result.count("output")) {
        std::ofstream outputFile(result["output"].as<std::string>());
        if (!outputFile.is_open()) {
            std::cerr << "Error: Could not open output file." << std::endl;
            free_fn(encodedResult);
            return 1;
        }
        outputFile << hexStream.str();
    } else {
        std::cout << hexStream.str() << std::endl;
    }
    free_fn(encodedResult);
    return 0;
}

inline int expy_cli_decode(
    decoded_data* (*decode_fn)(const uint8_t*, size_t),
    void (*free_fn)(decoded_data*),
    const cxxopts::ParseResult& result,
    const std::string& flag_value
) {
    using json = nlohmann::json;

    if (flag_value != "cmd" && result.count("input")) {
        std::cerr << "Error: Cannot decode string input with file input option." << std::endl;
        return 1;
    }

    std::string hexString;
    if (result.count("input")) {
        std::ifstream inputFile(result["input"].as<std::string>());
        if (!inputFile.is_open()) {
            std::cerr << "Error: Could not open input file." << std::endl;
            return 1;
        }
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        hexString = buffer.str();
    } else {
        hexString = flag_value;
    }

    if (hexString.length() % 2 != 0) {
        std::cerr << "Error: Invalid hex string length." << std::endl;
        return 1;
    }

    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hexString.length(); i += 2) {
        try {
            uint8_t byte = static_cast<uint8_t>(
                std::stoi(hexString.substr(i, 2), nullptr, 16));
            bytes.push_back(byte);
        } catch (const std::exception&) {
            std::cerr << "Error: Invalid hex string format." << std::endl;
            return 1;
        }
    }

    decoded_data* decoded = decode_fn(bytes.data(), bytes.size());
    if (decoded->status != 0) {
        std::cerr << "Error: libcbv2g decode failed (rc=" << decoded->status << ")." << std::endl;
        free_fn(decoded);
        return 1;
    }

    try {
        json j = json::parse(decoded->json);
        if (result.count("output")) {
            std::ofstream outputFile(result["output"].as<std::string>());
            if (!outputFile.is_open()) {
                std::cerr << "Error: Could not open output file." << std::endl;
                free_fn(decoded);
                return 1;
            }
            outputFile << j.dump(4);
        } else {
            std::cout << j.dump(4) << std::endl;
        }
    } catch (const json::parse_error&) {
        std::cerr << "Error: Failed to parse JSON output." << std::endl;
        free_fn(decoded);
        return 1;
    }
    free_fn(decoded);
    return 0;
}
