"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

from DINProcessor import DINProcessor
import json

# EXI string for PreChargeRequest
exiStringPrechargeReq = b'809a021050908c0c0c0c0c51514002808142807c0c0c0000'
exiBytesPrechargeReq = bytes.fromhex(exiStringPrechargeReq.decode('utf-8'))

# Json string for SessionSetupRes
jsonStringSessionSetupRes = """
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
decodedJson = dinProcessor.decode(exiBytesPrechargeReq)
print("Decoded result for PreChargeRequest:\n", json.dumps(decodedJson, indent=4))

# Encode the json string
encodedEXIResult = dinProcessor.encode(json.loads(jsonStringSessionSetupRes))
print("Encoded EXI for SessionSetupRes:", encodedEXIResult.hex())
print("\n\n")


from AppHandshakeProcessor import AppHandshakeProcessor

# EXI string for supportedAppProtocolReq
exiStringSupportedAppProtocolReq = b'8000dbab9371d3234b71d1b981899189d191818991d26b9b3a232b30020000040401b75726e3a7465736c613a64696e3a323031383a4d736744656600001c0100080'
exiBytesSupportedAppProtocolReq = bytes.fromhex(exiStringSupportedAppProtocolReq.decode('utf-8'))

# Json string for supportedAppProtocolRes
jsonStringSupportedAppProtocolRes = """
{
    "supportedAppProtocolRes": {
        "ResponseCode": 0,
        "SchemaID": 1
    }
}"""

# Create instance of AppHandshakeProcessor
appHandshakeProcessor = AppHandshakeProcessor()

# Decode the EXI bytes for supportedAppProtocolReq
decodedJsonSupportedAppProtocolReq = appHandshakeProcessor.decode(exiBytesSupportedAppProtocolReq)
print("Decoded result for supportedAppProtocolReq:\n", json.dumps(decodedJsonSupportedAppProtocolReq, indent=4))

# Encode the json string for supportedAppProtocolRes
encodedEXIResultSupportedAppProtocolRes = appHandshakeProcessor.encode(json.loads(jsonStringSupportedAppProtocolRes))
print("Encoded EXI for supportedAppProtocolRes:", encodedEXIResultSupportedAppProtocolRes.hex())