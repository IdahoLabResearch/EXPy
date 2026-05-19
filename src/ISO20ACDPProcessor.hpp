/*
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
*/

// extern "C" surface for the ISO20_ACDP Namespace Processor. Symbol shape
// matches the other Processor `.so`s (see ISO20CommonProcessor.cpp,
// ISO2Processor.cpp); once #23 lands, this hand-written header and the
// matching .cpp collapse into a single codegen template.
//
// The ACDP schema defines only `exiDocument` (no exiFragment, no
// xmldsigFragment), so the fragment/xmldsig entry points are intentionally
// omitted; EXIProcessor.py's binder silently skips them and the Python
// methods raise NotImplementedError per ADR-0003.

#pragma once

#include <cstddef>
#include <cstdint>

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
