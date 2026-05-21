"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""

from ctypes import CDLL, POINTER, Structure, c_char_p, c_int, c_size_t, c_uint8, cast
import json
import os

from enum import Enum

from _version import __version__


class Namespace(Enum):
    SAP = "SupportedAppProtocol"
    DIN = "DIN"
    ISO2 = "ISO2"
    ISO20_COMMON = "ISO20Common"
    ISO20_AC = "ISO20AC"
    ISO20_DC = "ISO20DC"
    ISO20_WPT = "ISO20WPT"
    ISO20_ACDP = "ISO20ACDP"


class EncodeError(Exception):
    """Raised on a non-zero encode status from the C++ entry point.

    Two failure surfaces produce this: libcbv2g's encoder (e.g.
    ``EXI_ERROR__BITSTREAM_OVERFLOW``) and the C++ JSON layer (``json::parse``,
    marshaler accessors). See ADR-0006.

    Typed attributes (ADR-0014):

    - ``rc: int | None`` — integer return code from the C++ entry point.
      ``-1`` to ``-299`` are libcbv2g's ``EXI_ERROR__*`` constants; ``-1000``
      is ``EXPY_ERROR__MARSHALER_INPUT`` (caught nlohmann JSON exception at
      the marshaler boundary). ``None`` only when the exception is
      constructed manually outside the Processor entry points.
    - ``namespace: str | None`` — the ``Namespace`` member name
      (``"SAP"``, ``"DIN"``, ``"ISO2"``, ``"ISO20_COMMON"``, ``"ISO20_AC"``,
      ``"ISO20_DC"``, ``"ISO20_WPT"``, ``"ISO20_ACDP"``).
    - ``root: str | None`` — which libcbv2g root the failing call targeted:
      ``"exiDocument"``, ``"exiFragment"``, or ``"xmldsigFragment"``.

    Consumers discriminating on failure mode should branch on these
    attributes. The ``str(e)`` message format from ADR-0006 is preserved for
    human-readable logs but is informational, not contractual.
    """

    def __init__(self, *args, rc=None, namespace=None, root=None):
        super().__init__(*args)
        self.rc = rc
        self.namespace = namespace
        self.root = root


class DecodeError(Exception):
    """Raised on a non-zero decode status from the C++ entry point.

    Two failure surfaces produce this: libcbv2g's decoder (e.g. malformed EXI
    header) and the residual C++ JSON layer path on decode. See ADR-0006.

    Typed attributes (ADR-0014):

    - ``rc: int | None`` — integer return code from the C++ entry point.
      ``-1`` to ``-299`` are libcbv2g's ``EXI_ERROR__*`` constants; ``-1000``
      is ``EXPY_ERROR__MARSHALER_INPUT`` (caught nlohmann JSON exception at
      the marshaler boundary). ``None`` only when the exception is
      constructed manually outside the Processor entry points.
    - ``namespace: str | None`` — the ``Namespace`` member name
      (``"SAP"``, ``"DIN"``, ``"ISO2"``, ``"ISO20_COMMON"``, ``"ISO20_AC"``,
      ``"ISO20_DC"``, ``"ISO20_WPT"``, ``"ISO20_ACDP"``).
    - ``root: str | None`` — which libcbv2g root the failing call targeted:
      ``"exiDocument"``, ``"exiFragment"``, or ``"xmldsigFragment"``.

    Consumers discriminating on failure mode should branch on these
    attributes. The ``str(e)`` message format from ADR-0006 is preserved for
    human-readable logs but is informational, not contractual.
    """

    def __init__(self, *args, rc=None, namespace=None, root=None):
        super().__init__(*args)
        self.rc = rc
        self.namespace = namespace
        self.root = root


# Matches `EXPY_ERROR__MARSHALER_INPUT` in include/common.hpp. Surfaced by the
# C++ entry points whenever an nlohmann::json exception (parse failure, missing
# key, wrong-typed accessor) is caught at the extern "C" boundary; distinct
# from libcbv2g's own rc range (-1 .. -299) so callers and tests can tell the
# two surfaces apart.
_EXPY_ERROR_MARSHALER_INPUT = -1000


# Maps the ctypes entry-point symbol to (namespace_tag, root_tag) for error
# messages. The namespace_tag is the Namespace name; the root_tag identifies
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
    def __init__(self, namespace: Namespace):
        namespaceString = namespace.value
        soFilePath = os.path.abspath(__file__ + f"/../build/lib-{namespaceString}Processor.so")
        self.lib = CDLL(soFilePath)
        self._namespace = namespace.name

        self._bind_decoder("decodeFromPythonBytes")
        self._bind_encoder("encodeFromPythonDict")

        self.lib.free_encoded_data.argtypes = [POINTER(encoded_data)]
        self.lib.free_encoded_data.restype = None
        self.lib.free_decoded_data.argtypes = [POINTER(decoded_data)]
        self.lib.free_decoded_data.restype = None

        # Fragment / XmldsigFragment entry points only exist on Processors whose
        # underlying libcbv2g schema defines those root types. Bind the Python
        # method to the instance only when the corresponding ctypes symbol is
        # present; otherwise the attribute is absent and `hasattr` returns False
        # (ADR-0014).
        self._maybe_bind_decode("decode_fragment", "decode_fragment_FromPythonBytes")
        self._maybe_bind_encode("encode_fragment", "encode_fragment_FromPythonDict")
        self._maybe_bind_decode("decode_xmldsig", "decode_xmldsig_FromPythonBytes")
        self._maybe_bind_encode("encode_xmldsig", "encode_xmldsig_FromPythonDict")

    def _maybe_bind_decode(self, method_name: str, symbol: str) -> None:
        if not hasattr(self.lib, symbol):
            return
        self._bind_decoder(symbol)
        setattr(self, method_name, lambda exiBytes, _s=symbol: self._decode_with(_s, exiBytes))

    def _maybe_bind_encode(self, method_name: str, symbol: str) -> None:
        if not hasattr(self.lib, symbol):
            return
        self._bind_encoder(symbol)
        setattr(self, method_name, lambda jsonObj, _s=symbol: self._encode_with(_s, jsonObj))

    def _format_error(self, root: str, status: int) -> str:
        if status == _EXPY_ERROR_MARSHALER_INPUT:
            return f"{self._namespace} {root}: invalid JSON input (rc={status})"
        return f"{self._namespace} {root}: libcbv2g rc={status}"

    def _bind_decoder(self, symbol: str) -> None:
        fn = getattr(self.lib, symbol)
        fn.argtypes = [POINTER(c_uint8), c_size_t]
        fn.restype = POINTER(decoded_data)

    def _bind_encoder(self, symbol: str) -> None:
        fn = getattr(self.lib, symbol)
        fn.argtypes = [c_char_p]
        fn.restype = POINTER(encoded_data)

    def _decode_with(self, symbol: str, exiBytes: bytes) -> dict:
        fn = getattr(self.lib, symbol)
        c_input_array = (c_uint8 * len(exiBytes))(*exiBytes)
        c_input_array_size = c_size_t(len(exiBytes))
        c_input_array_ptr = cast(c_input_array, POINTER(c_uint8))
        res = fn(c_input_array_ptr, c_input_array_size)
        try:
            status = res.contents.status
            if status != 0:
                root = _ROOT_BY_SYMBOL[symbol]
                raise DecodeError(
                    self._format_error(root, status),
                    rc=status,
                    namespace=self._namespace,
                    root=root,
                )
            return json.loads(res.contents.json.decode('utf-8'))
        finally:
            self.lib.free_decoded_data(res)

    def _encode_with(self, symbol: str, jsonObj: dict) -> bytes:
        fn = getattr(self.lib, symbol)
        c_input = c_char_p(json.dumps(jsonObj).encode('utf-8'))
        result = fn(c_input)
        try:
            status = result.contents.status
            if status != 0:
                root = _ROOT_BY_SYMBOL[symbol]
                raise EncodeError(
                    self._format_error(root, status),
                    rc=status,
                    namespace=self._namespace,
                    root=root,
                )
            return bytes(result.contents.buffer[:result.contents.size])
        finally:
            self.lib.free_encoded_data(result)

    def decode(self, exiBytes: bytes) -> dict:
        return self._decode_with("decodeFromPythonBytes", exiBytes)

    def encode(self, jsonObj: dict) -> bytes:
        return self._encode_with("encodeFromPythonDict", jsonObj)


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