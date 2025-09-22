"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

from ctypes import *
import json
import os

from enum import Enum

class ProtocolEnum(Enum):
    HANDSHAKE = "AppHandshake"
    DIN = "DIN"
    ISO2 = "ISO2"
    ISO20 = "ISO20"

class EXIProcessor():
    def __init__(self, protocol: ProtocolEnum):
        protocolString = protocol.value
        soFilePath = os.path.abspath(__file__ + f"/../build/lib-{protocolString}Processor.so")
        self.lib = CDLL(soFilePath)

        self.lib.decodeFromPythonBytes.argtypes = [POINTER(c_uint8), c_size_t]
        self.lib.decodeFromPythonBytes.restype = c_char_p

        self.lib.encodeFromPythonDict.argtypes = [c_char_p]
        self.lib.encodeFromPythonDict.restype = POINTER(encoded_data)
        
        self.lib.free_encoded_data.argtypes = [POINTER(encoded_data)]
        self.lib.free_encoded_data.restype = None

    def decode(self, exiBytes: bytes) -> dict:
        c_input_array = (c_uint8 * len(exiBytes))(*exiBytes)
        c_input_array_size = c_size_t(len(exiBytes))
        c_input_array_ptr = cast(c_input_array, POINTER(c_uint8))

        res = self.lib.decodeFromPythonBytes(c_input_array_ptr, c_input_array_size)

        jsonObj = json.loads(res.decode('utf-8'))

        return jsonObj
    
    def encode(self, jsonObj: dict) -> bytes:
        jsonObjStr = json.dumps(jsonObj)
        c_input = c_char_p(jsonObjStr.encode('utf-8'))

        result = self.lib.encodeFromPythonDict(c_input)
        if not result:
            return None

        # Copy the data before freeing
        encoded_bytes = bytes(result.contents.buffer[:result.contents.size])
        self.lib.free_encoded_data(result)
        
        return encoded_bytes

class encoded_data(Structure):
    _fields_ = [
        ("buffer", POINTER(c_uint8)),
        ("size", c_size_t)
    ]