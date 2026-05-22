"""V2Gjson roundtrip suite: one constructor-built Document per Namespace (issue #33).

For each of the 8 Namespaces, build a minimal valid message *using V2Gjson
constructors* (not raw dicts), run it through ``encode`` → ``decode``, and
assert strict equality with the constructor-built input. This pins the
contract that V2Gjson's emitted dict shape exactly matches what its sibling
Processor's ``encode`` consumes — the most likely failure mode of any future
codegen change.

Companion to ``test_v2gjson_smoke.py`` (which owns the package-level surface
contracts: wildcard-leak guard, version exposure, codegen-banner check,
example.py exerciser).
"""

from __future__ import annotations

import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))

from conftest import assert_roundtrip  # noqa: E402


def test_sap_supported_app_protocol_req_roundtrip():
    from expy.v2gjson import sap

    protocols = [
        sap.AppProtocolType(
            ProtocolNamespace="urn:din:70121:2012:MsgDef",
            VersionNumberMajor=2,
            VersionNumberMinor=0,
            SchemaID=1,
            Priority=1,
        ),
    ]
    payload = {"supportedAppProtocolReq": sap.supportedAppProtocolReq(AppProtocol=protocols)}
    assert_roundtrip("SAP", payload)


def test_din_session_stop_req_roundtrip():
    from expy.v2gjson import din

    header = din.MessageHeaderType(SessionID=bytearray(b"DECAFBAD"))
    body = din.BodyType(SessionStopReq=din.SessionStopType())
    payload = {"Header": header, "Body": body}
    assert_roundtrip("DIN", payload)


def test_iso2_session_stop_req_roundtrip():
    from expy.v2gjson import iso2

    header = iso2.MessageHeaderType(SessionID=bytearray(b"DECAFBAD"))
    body = iso2.BodyType(
        SessionStopReq=iso2.SessionStopReqType(
            ChargingSession=iso2.chargingSessionType.Terminate
        )
    )
    payload = {"Header": header, "Body": body}
    assert_roundtrip("ISO2", payload)


def test_iso20_common_session_setup_req_roundtrip():
    from expy.v2gjson import iso20_common

    header = iso20_common.MessageHeaderType(
        SessionID=bytearray(b"DECAFBAD"), TimeStamp=0
    )
    payload = {
        "SessionSetupReq": iso20_common.SessionSetupReqType(
            Header=header, EVCCID="EVCC01"
        )
    }
    assert_roundtrip("ISO20_COMMON", payload)


def test_iso20_ac_charge_parameter_discovery_req_roundtrip():
    from expy.v2gjson import iso20_ac

    header = iso20_ac.MessageHeaderType(
        SessionID=bytearray(b"DECAFBAD"), TimeStamp=0
    )
    mode = iso20_ac.AC_CPDReqEnergyTransferModeType(
        EVMaximumChargePower=iso20_ac.RationalNumberType(Exponent=0, Value=1),
        EVMinimumChargePower=iso20_ac.RationalNumberType(Exponent=0, Value=1),
    )
    payload = {
        "AC_ChargeParameterDiscoveryReq": iso20_ac.AC_ChargeParameterDiscoveryReqType(
            Header=header, AC_CPDReqEnergyTransferMode=mode
        )
    }
    assert_roundtrip("ISO20_AC", payload)


def test_iso20_dc_charge_parameter_discovery_req_roundtrip():
    from expy.v2gjson import iso20_dc

    header = iso20_dc.MessageHeaderType(
        SessionID=bytearray(b"DECAFBAD"), TimeStamp=0
    )
    mode = iso20_dc.DC_CPDReqEnergyTransferModeType(
        EVMaximumChargePower=iso20_dc.RationalNumberType(Exponent=0, Value=1),
        EVMinimumChargePower=iso20_dc.RationalNumberType(Exponent=0, Value=1),
        EVMaximumChargeCurrent=iso20_dc.RationalNumberType(Exponent=0, Value=1),
        EVMinimumChargeCurrent=iso20_dc.RationalNumberType(Exponent=0, Value=1),
        EVMaximumVoltage=iso20_dc.RationalNumberType(Exponent=0, Value=1),
        EVMinimumVoltage=iso20_dc.RationalNumberType(Exponent=0, Value=1),
    )
    payload = {
        "DC_ChargeParameterDiscoveryReq": iso20_dc.DC_ChargeParameterDiscoveryReqType(
            Header=header, DC_CPDReqEnergyTransferMode=mode
        )
    }
    assert_roundtrip("ISO20_DC", payload)


def test_iso20_wpt_pairing_req_roundtrip():
    from expy.v2gjson import iso20_wpt

    header = iso20_wpt.MessageHeaderType(
        SessionID=bytearray(b"DECAFBAD"), TimeStamp=0
    )
    payload = {
        "WPT_PairingReq": iso20_wpt.WPT_PairingReqType(
            Header=header,
            EVProcessing=iso20_wpt.processingType.Finished,
            EVResultCode=iso20_wpt.WPT_EVResultType.EVResultSuccess,
        )
    }
    assert_roundtrip("ISO20_WPT", payload)


def test_iso20_acdp_connect_req_roundtrip():
    from expy.v2gjson import iso20_acdp

    header = iso20_acdp.MessageHeaderType(
        SessionID=bytearray(b"DECAFBAD"), TimeStamp=0
    )
    payload = {
        "ACDP_ConnectReq": iso20_acdp.ACDP_ConnectReqType(
            Header=header,
            EVElectricalChargingDeviceStatus=iso20_acdp.electricalChargingDeviceStatusType.State_B,
        )
    }
    assert_roundtrip("ISO20_ACDP", payload)
