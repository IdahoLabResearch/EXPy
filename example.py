"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved

    Demonstration of the v1.0 public API surface (ADR-0012):
      - `EXIProcessor` for encode/decode per Namespace.
      - `V2Gjson` builder modules — one per Namespace — for constructing
        EVerest-shape JSON payloads using typed constructors and enums.

    Each Namespace lives at `V2Gjson.<lowercased Namespace name>` and is
    imported as a module (`from V2Gjson import din`). Wildcard imports are
    avoided because every Namespace defines `MessageHeaderType`, `BodyType`,
    `V2G_Message`, etc., which silently collide on import order.
"""

import json

from EXIProcessor import EXIProcessor, Namespace
from V2Gjson import din, iso20_common, sap


# ---------------------------------------------------------------------------
# DIN: decode a real PreChargeReq, then build + encode a CableCheckReq using
# V2Gjson.din constructors.
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
print("Encoded DIN CableCheckReq EXI:", encodedDin.hex() if encodedDin else "FAILED")


# ---------------------------------------------------------------------------
# SAP: decode a supportedAppProtocolReq, then build + encode a Res payload.
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
print("Encoded SAP supportedAppProtocolRes EXI:", encodedSAP.hex() if encodedSAP else "FAILED")


# ---------------------------------------------------------------------------
# ISO 15118-20 Common: build a SessionSetupReq via V2Gjson.iso20_common.
# ISO-20 messages live at the top level (no V2G_Message envelope) — each
# is wrapped in a Body union, the same shape `EXIProcessor.encode` accepts.
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
