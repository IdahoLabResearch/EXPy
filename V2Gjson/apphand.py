"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""


from enum import Enum
from typing import Any


class responseCodeType(Enum):
    OK_SuccessfulNegotiation = 0,
    OK_SuccessfulNegotiationWithMinorDeviation = 1,
    Failed_NoNegotiation = 2


def AppProtocolType(ProtocolNamespace:str, VersionNumberMajor:int, VersionNumberMinor:int, SchemaID:int, SchemaVersion:int)->dict[str, Any]:
    """
    :param ProtocolNamespace: string
    :param VersionNumberMajor: uint32/unsignedInt
    :param VersionNumberMinor: uint32/unsignedInt
    :param SchemaID: uint8/unsignedByte
    :param SchemaVersion: uint8/unsignedByte
    """

    ProtocolNamespace_characters = [ord(c) for c in ProtocolNamespace]
    
    j:dict[str, Any] = {
        "ProtocolNamespace": {
            "characters": ProtocolNamespace_characters,
            "charactersLen": len(ProtocolNamespace_characters)
        },
        "VersionNumberMajor": VersionNumberMajor,
        "VersionNumberMinor": VersionNumberMinor,
        "SchemaID": SchemaID,
        "SchemaVersion": SchemaVersion
    }

    return j

def supportedAppProtocolReq(AppProtocol:list[dict[str, Any]])->dict[str, Any]:
    """
    :param AppProtocol: list[dict[AppProtocolType]]
    """

    j:dict[str, Any] = {
        "AppProtocol": {
            "arraySize": len(AppProtocol),
            "array": AppProtocol
        }
    }

    return j

def supportedAppProtocolRes(ResponseCode:responseCodeType, SchemaID:int|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param SchemaID: uint8/unsignedByte (optional)
    """

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value
    }

    if SchemaID is not None:
        j["SchemaID"] = SchemaID

    return j

def exiDocument(AppProtocolReq:dict[str, Any]|None=None, SupportedAppProtocolRes:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param AppProtocolReq: supportedAppProtocolReq (optional)
    :param SupportedAppProtocolRes: supportedAppProtocolRes (optional)
    """

    j:dict[str, Any] = {}

    if AppProtocolReq is not None:
        j["AppProtocolReq"] = AppProtocolReq

    if SupportedAppProtocolRes is not None:
        j["SupportedAppProtocolRes"] = SupportedAppProtocolRes

    return j