"""Public-surface packaging contracts per ADR-0013.

Pins the v1.0 install layout: the package is named ``expy``, the Processor and
Namespace live at the top level, the V2Gjson builder modules live under
``expy.v2gjson``, and the canonical ``__version__`` is re-exported at the
package level. The ctypes loader resolves each per-Namespace ``.so`` relative
to the installed package, not via a ``./build/`` path on disk.

Per-Namespace round-trip behavior is covered exhaustively in the sibling
``test_*_generated_roundtrip.py`` modules; here we only confirm one end-to-end
path through the new import surface.
"""

from __future__ import annotations


def test_top_level_imports_resolve_and_round_trip():
    from expy import EXIProcessor, Namespace
    from expy.v2gjson import din

    processor = EXIProcessor(Namespace.DIN)
    header = din.MessageHeaderType(SessionID=bytearray(b"DECAFBAD"))
    status = din.DC_EVStatusType(
        EVReady=1, EVErrorCode=din.DC_EVErrorCodeType.NO_ERROR, EVRESSSOC=85
    )
    body = din.BodyType(CableCheckReq=din.CableCheckReqType(DC_EVStatus=status))
    payload = din.V2G_Message(Header=header, Body=body)

    exi = processor.encode(payload)
    decoded = processor.decode(exi)

    assert decoded == payload


def test_expy_package_exposes_version():
    from expy import __version__

    assert __version__ == "1.0.0"


def test_v2gjson_submodules_importable_under_expy():
    from expy.v2gjson import (
        sap,
        din,
        iso2,
        iso20_common,
        iso20_ac,
        iso20_dc,
        iso20_wpt,
        iso20_acdp,
    )

    modules = (sap, din, iso2, iso20_common, iso20_ac, iso20_dc, iso20_wpt, iso20_acdp)
    assert all(m is not None for m in modules)
