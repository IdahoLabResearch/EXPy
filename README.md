# EXI for Python (EXPy)

The project provides a C/C++ to Python interface to the LF Energy EVerest EXI encoding
library (libcbv2g).

## Overview

EXPy is a Python binding for the EXI (Efficient XML Interchange) format, leveraging the LF Energy EVerest implementation. It enables efficient XML processing in Python applications while maintaining compatibility with C/C++ codebases.

There are also Python helper methods for creating V2G payloads from Python json objects.

## TODO

Implement V2G Specifications
- ~~Implement DIN 70121~~
- ~~Implement ISO 15118-2~~
- Implement ISO 15118-20

Add testing for each specification
- Implement DIN 70121 Testing (In Progress)
- Implement ISO 15118-2 Testing
- Implement ISO 15118-20 Testing

## Requirements

- Python
- C/C++
- CMake
- Ninja

## Installation

```bash
git clone --recurse-submodules <giturl>
cd EXPy
make
```

## Usage from CMD

For CMD help output
```bash
./build/DINProcessor -h
```

```
Encode and decode DIN EXI messages
If no arguement specified for input/outfile, will default to stdin/stdout
MUST use double quotes (") and lower case true/false
Usage:
  ./DINProcessor --[encode/decode]='STRING_OPTIONAL' -i FILE_INPUT -o FILE_OUTPUT
  ./DINProcessor --decode='809a021050908c0c0c0c0c51e000040080'
  ./DINProcessor -e -i input.json -o output.txt
  ./DINProcessor [OPTION...]

  -h, --help                 Show help
  -e, --encode [=arg(=cmd)]  Encode EXI message from JSON to EXI bytes
  -d, --decode [=arg(=cmd)]  Decode EXI message from EXI bytes to JSON
  -i, --input arg            Input file to read JSON or EXI bytes from
  -o, --output arg           Output file to write JSON or EXI bytes to
```

### Encoding
#### User Input
```bash
./build/DINProcessor --encode='{"Body": {"SessionSetupRes": {"EVSEID": {"bytes": [0, 127, 0, 0, 16, 255, 119, 61, 253, 127, 0, 0, 90, 120, 167, 154, 129, 127, 0, 0, 92, 191, 184, 154, 129, 127, 0, 0, 176, 220, 184, 154], "bytesLen": 1}, "ResponseCode": 0, "isUsed": true}}, "Header": {"SessionID": {"bytes": [65, 66, 66, 48, 48, 48, 48, 49], "bytesLen": 8}}}'
```
```bash
./build/DINProcessor -e -i inputFile.json
```
#### Program Output
```
809a021050908c0c0c0c0c51e000040080
```

### Decoding
#### User Input
```bash
./build/DINProcessor --decode='809a021050908c0c0c0c0c51e000040080'
```
```bash
./build/DINProcessor -d -i inputFile.txt
```
#### Program Output
```
{
    "Body": {
        "SessionSetupRes": {
            "EVSEID": {
                "bytes": [
                    0
                ],
                "bytesLen": 1
            },
            "ResponseCode": 0,
            "isUsed": true
        }
    },
    "Header": {
        "SessionID": {
            "bytes": [
                65,
                66,
                66,
                48,
                48,
                48,
                48,
                49
            ],
            "bytesLen": 8
        }
    }
}
```

## Usage from Python

A more comprehensive example is shown in the example.py script. A small snipped of this script is shown below.

#### Python File

```python
from EXIProcessor import *
import json

# EXI string for PreChargeRequest
exiString = b'809a021050908c0c0c0c0c51514002808142807c0c0c0000'
exiBytes = bytes.fromhex(exiString.decode('utf-8'))

# Json string for SessionSetupRes
jsonString = """
{
    "Body": {
        "SessionSetupRes": {
            "EVSEID": {
                "bytes": [
                    0
                ],
                "bytesLen": 1
            },
            "ResponseCode": 0,
            "isUsed": true
        }
    },
    "Header": {
        "SessionID": {
            "bytes": [
                65,
                66,
                66,
                48,
                48,
                48,
                48,
                49
            ],
            "bytesLen": 8
        }
    }
}"""

# Create instance of EXIProcessor
dinProcessor = EXIProcessor(ProtocolEnum.DIN)

# Decode the EXI bytes
decodedJson = dinProcessor.decode(exiBytes)
print("Decoded result:", json.dumps(decodedJson, indent=4))

# Encode the json string
encodedEXIResult = dinProcessor.encode(json.loads(jsonString))
print("Encoded EXi for SessionSetupRes:", encodedEXIResult.hex())
```
#### Program Output

```
Decoded result: 
{
    "Body": {
        "PreChargeReq": {
            "DC_EVStatus": {
                "EVErrorCode": 0,
                "EVRESSSOC": 10,
                "EVReady": 1
            },
            "EVTargetCurrent": {
                "Multiplier": 0,
                "Unit": 3,
                "Value": 0
            },
            "EVTargetVoltage": {
                "Multiplier": -1,
                "Unit": 5,
                "Value": 4000
            },
            "isUsed": true
        }
    },
    "Header": {
        "SessionID": {
            "bytes": [
                65,
                66,
                66,
                48,
                48,
                48,
                48,
                49
            ],
            "bytesLen": 8
        }
    }
}
Encoded EXI for SessionSetupRes: 809a021050908c0c0c0c0c51e000040080
```