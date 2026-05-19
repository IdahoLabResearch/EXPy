"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

from ctypes import CDLL, POINTER, Structure, c_char_p, c_int, c_size_t, c_uint8, cast
import json
import os

from enum import Enum


class ProtocolEnum(Enum):
    SAP = "SupportedAppProtocol"
    DIN = "DIN"
    ISO2 = "ISO2"
    ISO20 = "ISO20"


class EncodeError(Exception):
    """Raised when libcbv2g reports a non-zero return code from an encode call."""


class DecodeError(Exception):
    """Raised when libcbv2g reports a non-zero return code from a decode call."""


# Maps the ctypes entry-point symbol to (namespace_tag, root_tag) for error
# messages. The namespace_tag is the ProtocolEnum name; the root_tag identifies
# which libcbv2g root the call targets (exiDocument / exiFragment / xmldsigFragment).
_ROOT_BY_SYMBOL = {
    "decodeFromPythonBytes": "exiDocument",
    "encodeFromPythonDict": "exiDocument",
    "decode_fragment_FromPythonBytes": "exiFragment",
    "encode_fragment_FromPythonDict": "exiFragment",
    "decode_xmldsig_FromPythonBytes": "xmldsigFragment",
    "encode_xmldsig_FromPythonDict": "xmldsigFragment",
}


class EXIProcessor():
    def __init__(self, protocol: ProtocolEnum):
        protocolString = protocol.value
        soFilePath = os.path.abspath(__file__ + f"/../build/lib-{protocolString}Processor.so")
        self.lib = CDLL(soFilePath)
        self._namespace = protocol.name

        self._bind_decoder("decodeFromPythonBytes")
        self._bind_encoder("encodeFromPythonDict")

        self.lib.free_encoded_data.argtypes = [POINTER(encoded_data)]
        self.lib.free_encoded_data.restype = None
        self.lib.free_decoded_data.argtypes = [POINTER(decoded_data)]
        self.lib.free_decoded_data.restype = None

        # Fragment / XmldsigFragment entry points only exist on Processors whose
        # underlying libcbv2g schema defines those root types (ISO-2 and the
        # ISO-20 namespaces). On other Processors (SAP, DIN) the symbol is
        # missing and the binder silently skips the wiring — the corresponding
        # Python method then raises NotImplementedError.
        self._bind_decoder("decode_fragment_FromPythonBytes")
        self._bind_encoder("encode_fragment_FromPythonDict")
        self._bind_decoder("decode_xmldsig_FromPythonBytes")
        self._bind_encoder("encode_xmldsig_FromPythonDict")

    def _bind_decoder(self, symbol: str) -> None:
        try:
            fn = getattr(self.lib, symbol)
        except AttributeError:
            return
        fn.argtypes = [POINTER(c_uint8), c_size_t]
        fn.restype = POINTER(decoded_data)

    def _bind_encoder(self, symbol: str) -> None:
        try:
            fn = getattr(self.lib, symbol)
        except AttributeError:
            return
        fn.argtypes = [c_char_p]
        fn.restype = POINTER(encoded_data)

    def _decode_with(self, symbol: str, exiBytes: bytes) -> dict:
        try:
            fn = getattr(self.lib, symbol)
        except AttributeError:
            raise NotImplementedError(
                f"this Processor does not expose {symbol}"
            )
        c_input_array = (c_uint8 * len(exiBytes))(*exiBytes)
        c_input_array_size = c_size_t(len(exiBytes))
        c_input_array_ptr = cast(c_input_array, POINTER(c_uint8))
        res = fn(c_input_array_ptr, c_input_array_size)
        try:
            status = res.contents.status
            if status != 0:
                root = _ROOT_BY_SYMBOL[symbol]
                raise DecodeError(
                    f"{self._namespace} {root}: libcbv2g rc={status}"
                )
            return json.loads(res.contents.json.decode('utf-8'))
        finally:
            self.lib.free_decoded_data(res)

    def _encode_with(self, symbol: str, jsonObj: dict) -> bytes:
        try:
            fn = getattr(self.lib, symbol)
        except AttributeError:
            raise NotImplementedError(
                f"this Processor does not expose {symbol}"
            )
        c_input = c_char_p(json.dumps(jsonObj).encode('utf-8'))
        result = fn(c_input)
        try:
            status = result.contents.status
            if status != 0:
                root = _ROOT_BY_SYMBOL[symbol]
                raise EncodeError(
                    f"{self._namespace} {root}: libcbv2g rc={status}"
                )
            return bytes(result.contents.buffer[:result.contents.size])
        finally:
            self.lib.free_encoded_data(result)

    def decode(self, exiBytes: bytes) -> dict:
        return self._decode_with("decodeFromPythonBytes", exiBytes)

    def encode(self, jsonObj: dict) -> bytes:
        return self._encode_with("encodeFromPythonDict", jsonObj)

    def decode_fragment(self, exiBytes: bytes) -> dict:
        return self._decode_with("decode_fragment_FromPythonBytes", exiBytes)

    def encode_fragment(self, jsonObj: dict) -> bytes:
        return self._encode_with("encode_fragment_FromPythonDict", jsonObj)

    def decode_xmldsig(self, exiBytes: bytes) -> dict:
        return self._decode_with("decode_xmldsig_FromPythonBytes", exiBytes)

    def encode_xmldsig(self, jsonObj: dict) -> bytes:
        return self._encode_with("encode_xmldsig_FromPythonDict", jsonObj)


class encoded_data(Structure):
    _fields_ = [
        ("buffer", POINTER(c_uint8)),
        ("size", c_size_t),
        ("status", c_int),
    ]


class decoded_data(Structure):
    _fields_ = [
        ("json", c_char_p),
        ("status", c_int),
    ]