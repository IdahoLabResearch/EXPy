"""EXPy: Python and C/C++ bindings over LF Energy EVerest's libcbv2g.

Public API surface (ADR-0013, ADR-0014):

- :class:`EXIProcessor` — per-Namespace EXI encode/decode.
- :class:`Namespace` — dispatch key identifying which schema a message belongs to.
- :class:`EncodeError`, :class:`DecodeError` — typed error contract (ADR-0006, ADR-0014).
- :mod:`expy.v2gjson` — per-Namespace builder modules emitting EVerest-shape JSON.
"""

from ._version import __version__
from .processor import DecodeError, EncodeError, EXIProcessor, Namespace

__all__ = [
    "__version__",
    "DecodeError",
    "EncodeError",
    "EXIProcessor",
    "Namespace",
]
