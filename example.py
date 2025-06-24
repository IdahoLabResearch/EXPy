"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

from DINProcessor import DINProcessor
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

# Create instance of DINProcessor
dinProcessor = DINProcessor()

# Decode the EXI bytes
decodedJson = dinProcessor.decode(exiBytes)
print("Decoded result:\n", json.dumps(decodedJson, indent=4))

# Encode the json string
encodedEXIResult = dinProcessor.encode(json.loads(jsonString))
print("Encoded EXI for SessionSetupRes:", encodedEXIResult.hex())