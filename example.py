"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved

    Demonstration of the v1.0 public API surface (ADR-0013, ADR-0014):

      - ``EXIProcessor(namespace=...)`` — per-Namespace EXI codec. Up to six
        methods are bound at construction time:
        ``encode`` / ``decode`` (Document root),
        ``encode_fragment`` / ``decode_fragment`` (Fragment root),
        ``encode_xmldsig`` / ``decode_xmldsig`` (XmldsigFragment root).

      - Fragment and XmldsigFragment methods are **absent** on Namespaces
        whose libcbv2g schema does not define the corresponding root (SAP
        and DIN today). Probe with ``hasattr(processor, "encode_fragment")``
        — direct attribute access on an absent method raises ``AttributeError``.

      - ``V2Gjson`` builder modules — one per Namespace at
        ``expy.v2gjson.<lowercased Namespace name>`` — exposing typed
        constructors and enums that emit EVerest-shape JSON ready to pass to
        ``EXIProcessor.encode`` / ``.encode_fragment`` / ``.encode_xmldsig``.

      - Top-level envelope shape varies per Namespace:
        DIN and ISO-2 use ``<ns>.V2G_Message(Header=..., Body=...)`` because
        the schemas define a ``V2G_Message`` complex type. SAP and the five
        ISO-20 Namespaces have no ``V2G_Message`` — top-level messages are
        hand-built raw dicts (``{"<MessageName>": ...}`` for SAP,
        ``{"Body": {"<MessageName>": ...}}`` for ISO-20).

    V2Gjson identifiers mirror libcbv2g verbatim, including non-PEP-8 casing
    (``costKindType``, ``RelativeTimeIntervalType``, ``EVSE_NotReady``).
    Module imports are explicit (``from expy.v2gjson import din, iso2``);
    wildcard imports are avoided because every Namespace defines
    ``MessageHeaderType``, ``BodyType``, ``V2G_Message``, etc., which
    silently collide on import order.
"""

import json

from expy import EXIProcessor, Namespace
from expy.v2gjson import din, iso2, iso20_common, sap


# ---------------------------------------------------------------------------
# DIN: decode a real PreChargeReq Document, then build + encode a
# CableCheckReq using V2Gjson.din constructors. DIN wraps top-level messages
# in din.V2G_Message(Header=..., Body=...).
# ---------------------------------------------------------------------------

dinProcessor = EXIProcessor(Namespace.DIN)

exiBytesPrechargeReq = bytes.fromhex("809a021050908c0c0c0c0c51514002808142807c0c0c0000")
decodedJson = dinProcessor.decode(exiBytesPrechargeReq)
print("Decoded DIN PreChargeReq:\n", json.dumps(decodedJson, indent=4))

dinHeader = din.MessageHeaderType(SessionID=bytearray(b"DECAFBAD"))
dinStatus = din.DC_EVStatusType(
    EVReady=1, EVErrorCode=din.DC_EVErrorCodeType.NO_ERROR, EVRESSSOC=85
)
dinBody = din.BodyType(CableCheckReq=din.CableCheckReqType(DC_EVStatus=dinStatus))
dinMessage = din.V2G_Message(Header=dinHeader, Body=dinBody)
print("\nBuilt DIN CableCheckReq via V2Gjson:\n", json.dumps(dinMessage, indent=4))

encodedDin = dinProcessor.encode(dinMessage)
print("Encoded DIN CableCheckReq EXI:", encodedDin.hex())

# DIN has no Fragment or XmldsigFragment root in its libcbv2g schema, so the
# corresponding methods are absent on the instance. Feature-detect with
# hasattr — never with try/except AttributeError on a guessed name.
assert not hasattr(dinProcessor, "encode_fragment")
assert not hasattr(dinProcessor, "encode_xmldsig")


# ---------------------------------------------------------------------------
# SAP: decode a supportedAppProtocolReq, then build + encode a Res payload.
# SAP has no V2G_Message — top-level messages are hand-built raw dicts.
# ---------------------------------------------------------------------------

sapProcessor = EXIProcessor(Namespace.SAP)

exiBytesSAP = bytes.fromhex(
    "8000dbab9371d3234b71d1b981899189d191818991d26b9b3a232b30020000040401"
    "b75726e3a7465736c613a64696e3a323031383a4d736744656600001c0100080"
)
decodedSAP = sapProcessor.decode(exiBytesSAP)
print("\nDecoded SAP supportedAppProtocolReq:\n", json.dumps(decodedSAP, indent=4))

sapResMessage = {
    "supportedAppProtocolRes": sap.supportedAppProtocolRes(
        ResponseCode=sap.responseCodeType.OK_SuccessfulNegotiation,
        SchemaID=1,
    )
}
encodedSAP = sapProcessor.encode(sapResMessage)
print("Encoded SAP supportedAppProtocolRes EXI:", encodedSAP.hex())


# ---------------------------------------------------------------------------
# ISO-2: build, encode, and decode a Fragment (AuthorizationReq) using the
# six-method API. Fragment / XmldsigFragment roots are required for PnC
# signature workflows — they encode a sub-element of a Document standalone.
# ---------------------------------------------------------------------------

iso2Processor = EXIProcessor(Namespace.ISO2)

# Probe the feature surface — these are bound on ISO-2 because the iso_2
# libcbv2g schema defines exiFragment and xmldsigFragment roots.
assert hasattr(iso2Processor, "encode_fragment")
assert hasattr(iso2Processor, "decode_fragment")
assert hasattr(iso2Processor, "encode_xmldsig")

iso2AuthReq = iso2.AuthorizationReqType(GenChallenge=bytearray(16))
iso2Fragment = iso2.exiFragment(AuthorizationReq=iso2AuthReq)
print(
    "\nBuilt ISO-2 AuthorizationReq Fragment via V2Gjson:\n",
    json.dumps(iso2Fragment, indent=4),
)

encodedIso2Fragment = iso2Processor.encode_fragment(iso2Fragment)
print("Encoded ISO-2 AuthorizationReq Fragment EXI:", encodedIso2Fragment.hex())
decodedIso2Fragment = iso2Processor.decode_fragment(encodedIso2Fragment)
print(
    "Round-tripped ISO-2 AuthorizationReq Fragment:\n",
    json.dumps(decodedIso2Fragment, indent=4),
)


# ---------------------------------------------------------------------------
# ISO 15118-20 Common: build a SessionSetupReq via V2Gjson.iso20_common.
# ISO-20 has no V2G_Message — top-level messages are hand-built raw dicts
# of shape {"Body": {"<MessageName>": ...}}.
# ---------------------------------------------------------------------------

iso20Header = iso20_common.MessageHeaderType(
    SessionID=bytearray(b"DECAFBAD"), TimeStamp=1700000000
)
iso20SessionSetup = iso20_common.SessionSetupReqType(
    Header=iso20Header, EVCCID="EVCC-001"
)
iso20Message = {"Body": {"SessionSetupReq": iso20SessionSetup}}
print(
    "\nBuilt ISO-20 Common SessionSetupReq via V2Gjson:\n",
    json.dumps(iso20Message, indent=4),
)
