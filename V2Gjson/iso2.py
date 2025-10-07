"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""


from enum import Enum
from typing import Any


class costKindType(Enum):
    relativePricePercentage = 0
    RenewableGenerationPercentage = 1
    CarbonDioxideEmission = 2


class unitSymbolType(Enum):
    h = 0
    m = 1
    s = 2
    A = 3
    V = 4
    W = 5
    Wh = 6


class DC_EVErrorCodeType(Enum):
    NO_ERROR = 0
    FAILED_RESSTemperatureInhibit = 1
    FAILED_EVShiftPosition = 2
    FAILED_ChargerConnectorLockFault = 3
    FAILED_EVRESSMalfunction = 4
    FAILED_ChargingCurrentdifferential = 5
    FAILED_ChargingVoltageOutOfRange = 6
    Reserved_A = 7
    Reserved_B = 8
    Reserved_C = 9
    FAILED_ChargingSystemIncompatibility = 10
    NoData = 11
    

class faultCodeType(Enum):
    ParsingError = 0
    NoTLSRootCertificatAvailable = 1
    UnknownError = 2


class EVSENotificationType(Enum):
    None_ = 0
    StopCharging = 1
    ReNegotiation = 2


class isolationLevelType(Enum):
    Invalid = 0
    Valid = 1
    Warning = 2
    Fault = 3
    No_IMD = 4


class serviceCategoryType(Enum):
    EVCharging = 0
    Internet = 1
    ContractCertificate = 2
    OtherCustom = 3


class DC_EVSEStatusCodeType(Enum):
    EVSE_NotReady = 0
    EVSE_Ready = 1
    EVSE_Shutdown = 2
    EVSE_UtilityInterruptEvent = 3
    EVSE_IsolationMonitoringActive = 4
    EVSE_EmergencyShutdown = 5
    EVSE_Malfunction = 6
    Reserved_8 = 7
    Reserved_9 = 8
    Reserved_A = 9
    Reserved_B = 10
    Reserved_C = 11


class chargeProgressType(Enum):
    Start = 0
    Stop = 1
    Renegotiate = 2


class responseCodeType(Enum):
    OK = 0
    OK_NewSessionEstablished = 1
    OK_OldSessionJoined = 2
    OK_CertificateExpiresSoon = 3
    FAILED = 4
    FAILED_SequenceError = 5
    FAILED_ServiceIDInvalid = 6
    FAILED_UnknownSession = 7
    FAILED_ServiceSelectionInvalid = 8
    FAILED_PaymentSelectionInvalid = 9
    FAILED_CertificateExpired = 10
    FAILED_SignatureError = 11
    FAILED_NoCertificateAvailable = 12
    FAILED_CertChainError = 13
    FAILED_ChallengeInvalid = 14
    FAILED_ContractCanceled = 15
    FAILED_WrongChargeParameter = 16
    FAILED_PowerDeliveryNotApplied = 17
    FAILED_TariffSelectionInvalid = 18
    FAILED_ChargingProfileInvalid = 19
    FAILED_MeteringSignatureNotValid = 20
    FAILED_NoChargeServiceSelected = 21
    FAILED_WrongEnergyTransferMode = 22
    FAILED_ContactorError = 23
    FAILED_CertificateNotAllowedAtThisEVSE = 24
    FAILED_CertificateRevoked = 25


class paymentOptionType(Enum):
    Contract = 0
    ExternalPayment = 1


class chargingSessionType(Enum):
    Terminante = 0
    Pause = 1


class EnergyTransferModeType(Enum):
    AC_single_phase_core = 0
    AC_three_phase_core = 1
    DC_core = 2
    DC_extended = 3
    DC_combo_core = 4
    DC_unique = 5


class EVSEProcessingType(Enum):
    Finished = 0
    Ongoing = 1
    Ongoing_WaitingForCustomerInteraction = 2


def CostType(costKind:costKindType, amount:int, amountMultiplier:int|None=None) -> dict[str, Any]:
    """
    :param costKind: costKindType
    :param amount: uint32/unsignedInt
    :param amountMultiplier: int8/byte (optional)
    """

    j:dict[str, Any] = {
        "CostKind": costKind.value,
        "Amount": amount
    }

    if amountMultiplier is not None:
        j["AmountMultiplier"] = amountMultiplier

    return j

def TransformType(Algorithm:str, ANY:bytearray|None=None, XPath:str|None=None)->dict[str, Any]:
    """
    :param Algorithm: string
    :param ANY: bytearray (optional)
    :param XPath: string (optional)
    """

    Algorithm_characters = [ord(x) for x in Algorithm]
    j:dict[str, Any] = {
        "Algorithm": {
            "charactersLen": len(Algorithm_characters),
            "characters": Algorithm_characters
        }
    }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytesLen": len(ANY_bytes),
            "bytes": ANY_bytes
        }
    
    if XPath is not None:
        XPath_characters = [ord(x) for x in XPath]
        j["XPath"] = {
            "charactersLen": len(XPath_characters),
            "characters": XPath_characters
        }
    
    return j

def IntervalType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str, Any] = {
        "_unused": _unused
    }

    return j

def TransformsType(Algorithm:str, ANY:bytearray|None=None, XPath:str|None=None)->dict[str, Any]:
    """
    :param Algorithm: string
    :param ANY: bytearray (optional)
    :param XPath: string (optional)
    """

    Transform = TransformType(Algorithm, ANY, XPath)
    j:dict[str, Any] = {
        "Transform": Transform
    }

    return j

def DSAKeyValueType(Y:bytearray, P:bytearray|None=None, Q:bytearray|None=None, G:bytearray|None=None, J:bytearray|None=None, Seed:bytearray|None=None, PgenCounter:bytearray|None=None)->dict[str, Any]:
    """
    :param Y: bytearray
    :param P: bytearray (optional)
    :param Q: bytearray (optional)
    :param G: bytearray (optional)
    :param J: bytearray (optional)
    :param Seed: bytearray (optional)
    :param PgenCounter: bytearray (optional)
    """

    Y_bytes = list(Y)
    j:dict[str, Any] = {
        "Y": {
            "bytes": Y_bytes,
            "bytesLen": len(Y_bytes)
        }
    }

    if P is not None:
        P_bytes = list(P)
        j["P"] = {
            "bytes": P_bytes,
            "bytesLen": len(P_bytes)
        }

    if Q is not None:
        Q_bytes = list(Q)
        j["Q"] = {
            "bytes": Q_bytes,
            "bytesLen": len(Q_bytes)
        }
    
    if G is not None:
        G_bytes = list(G)
        j["G"] = {
            "bytes": G_bytes,
            "bytesLen": len(G_bytes)
        }
    
    if J is not None:
        J_bytes = list(J)
        j["J"] = {
            "bytes": J_bytes,
            "bytesLen": len(J_bytes)
        }
    
    if Seed is not None:
        Seed_bytes = list(Seed)
        j["Seed"] = {
            "bytes": Seed_bytes,
            "bytesLen": len(Seed_bytes)
        }
    
    if PgenCounter is not None:
        PgenCounter_bytes = list(PgenCounter)
        j["PgenCounter"] = {
            "bytes": PgenCounter_bytes,
            "bytesLen": len(PgenCounter_bytes)
        }
    
    return j

def X509IssuerSerialType(X509IssuerName:str, X509SerialNumber:dict[str, Any])->dict[str, Any]:
    """
    :param X509IssuerName: string
    :param X509SerialNumber: dict[exi_signed_t]
    """

    X509IssuerName_chars = [ord(x) for x in X509IssuerName]
    j:dict[str,Any] = {
        "X509IssuerName": {
            "characters": X509IssuerName_chars,
            "charactersLen": len(X509IssuerName_chars)
        },
        "X509SerialNumber": X509SerialNumber
    }

    return j

def RelativeTimeIntervalType(start:int, duration:int|None=None)->dict[str, Any]:
    """
    :param start: uint32/unsignedInt
    :param duration: uint32/unsignedInt (optional)
    """

    j:dict[str, Any] = {
        "start": start
    }

    if duration is not None:
        j["duration"] = duration

    return j


def DigestMethodType(Algorithm:str, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param Algorithm: string
    :param ANY: bytearray (optional)
    """

    Algorithm_characters = [ord(x) for x in Algorithm]
    j:dict[str, Any] = {
        "Algorithm": {
            "charactersLen": len(Algorithm_characters),
            "characters": Algorithm_characters
        }
    }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytesLen": len(ANY_bytes),
            "bytes": ANY_bytes
        }
    
    return j

def RSAKeyValueType(Modulus:bytearray, Exponent:bytearray)->dict[str, Any]:
    """
    :param Modulus: bytearray
    :param Exponent: bytearray
    """

    Modulus_bytes = list(Modulus)
    Exponent_bytes = list(Exponent)
    j:dict[str, Any] = {
        "Modulus": {
            "bytes": Modulus_bytes,
            "bytesLen": len(Modulus_bytes)
        },
        "Exponent": {
            "bytes": Exponent_bytes,
            "bytesLen": len(Exponent_bytes)
        }
    }

    return j

def CanonicalizationMethodType(Algorithm:str, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param Algorithm: string
    :param ANY: bytearray (optional)
    """

    Algorithm_characters = [ord(x) for x in Algorithm]
    j:dict[str, Any] = {
        "Algorithm": {
            "charactersLen": len(Algorithm_characters),
            "characters": Algorithm_characters
        }
    }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytesLen": len(ANY_bytes),
            "bytes": ANY_bytes
        }
    
    return j

def SignatureMethodType(Algorithm:str, HMACOutputLength:dict[str, Any]|None=None, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param Algorithm: string
    :param HMACOutputLength: dict[exi_signed_t] (optional)
    :param ANY: bytearray (optional)
    """

    Algorithm_characters = [ord(x) for x in Algorithm]
    j: dict[str, Any] = {
        "Algorithm": {
            "charactersLen": len(Algorithm_characters),
            "characters": Algorithm_characters
        }
    }

    if HMACOutputLength is not None:
        j["HMACOutputLength"] = HMACOutputLength

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytesLen": len(ANY_bytes),
            "bytes": ANY_bytes
        }
    
    return j

def KeyValueType(DSAKeyValue:dict[str, Any]|None=None, RSAKeyValue:dict[str, Any]|None=None, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param DSAKeyValue: dict[DSAKeyValueType] (optional)
    :param RSAKeyValue: dict[RSAKeyValueType] (optional)
    :param ANY: bytearray (optional)
    """

    j:dict[str, Any] = {}

    if DSAKeyValue is not None:
        j["DSAKeyValue"] = DSAKeyValue

    if RSAKeyValue is not None:
        j["RSAKeyValue"] = RSAKeyValue

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytesLen": len(ANY_bytes),
            "bytes": ANY_bytes
        }

    return j

def PhysicalValueType(Multiplier:int, Unit:unitSymbolType, Value:int)->dict[str, Any]:
    """
    :param Multiplier: int8/byte
    :param Unit: unitSymbolType
    :param Value: int16/short
    """

    j:dict[str, Any] = {
        "Multiplier": Multiplier,
        "Unit": Unit.value,
        "Value": Value
    }

    return j

def ConsumptionCostType(startValue:dict[str, Any], Cost:list[dict])->dict[str, Any]:
    """
    :param startValue: dict[PhysicalValueType]
    :param Cost: list[dict[CostType]]
    """

    j:dict[str, Any] = {
        "Start": startValue,
        "Cost": Cost
    }

    return j

def PMaxScheduleEntryType(PMax:dict[str, Any], RelativeTimeInterval:dict[str, Any]|None=None, TimeInterval:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param Pmax: dict[PhysicalValueType]
    :param RelativeTimeInterval: dict[RelativeTimeIntervalType] (optional)
    :param TimeInterval: dict[IntervalType] (optional)
    """

    j:dict[str, Any] = {
        "PMax": PMax
    }

    if RelativeTimeInterval is not None:
        j["RelativeTimeInterval"] = RelativeTimeInterval

    if TimeInterval is not None:
        j["TimeInterval"] = TimeInterval

    return j

def SalesTariffEntryType(ConsumptionCost:list[dict], RelativeTimeInterval:dict[str, Any]|None=None, TimeInterval:dict[str, Any]|None=None, EPriceLevel:int|None=None)->dict[str, Any]:
    """
    :param ConsumptionCost: list[dict[ConsumptionCostType]]
    :param RelativeTimeInterval: dict[RelativeTimeIntervalType] (optional)
    :param TimeInterval: dict[IntervalType] (optional)
    :param EPriceLevel: int (optional)
    """

    j:dict[str, Any] = {
        "ConsumptionCost": ConsumptionCost
    }

    if RelativeTimeInterval is not None:
        j["RelativeTimeInterval"] = RelativeTimeInterval

    if TimeInterval is not None:
        j["TimeInterval"] = TimeInterval

    if EPriceLevel is not None:
        j["EPriceLevel"] = EPriceLevel

    return j

def ParameterType(Name:str, boolValue:int|None=None, byteValue:int|None=None, shortValue:int|None=None, intValue:int|None=None, physicalValue:dict[str, Any]|None=None, stringValue:str|None=None)->dict[str, Any]:
    """
    :param Name: string
    :param boolValue: boolean (optional)
    :param byteValue: int8/byte (optional)
    :param shortValue: int16/short (optional)
    :param intValue: int32/integer (optional)
    :param physicalValue: dict[PhysicalValueType] (optional)
    :param stringValue: string (optional)
    """

    j:dict[str, Any] = {
        "Name": Name
    }

    if boolValue is not None:
        j["boolValue"] = boolValue

    if byteValue is not None:
        j["byteValue"] = byteValue

    if shortValue is not None:
        j["shortValue"] = shortValue

    if intValue is not None:
        j["intValue"] = intValue

    if physicalValue is not None:
        j["physicalValue"] = physicalValue

    if stringValue is not None:
        stringValue_characters = [ord(x) for x in stringValue]
        j["stringValue"] = {
            "charactersLen": len(stringValue_characters),
            "characters": stringValue_characters
        }

    return j

def PMaxScheduleType(PMaxScheduleEntry:list[dict])->dict[str, Any]:
    """
    :param PMaxScheduleEntry: list[dict[PMaxScheduleEntryType]]
    """

    j:dict[str, Any] = {
        "PMaxScheduleEntry": {
            "arrayLen": len(PMaxScheduleEntry),
            "array": PMaxScheduleEntry
        }
    }

    return j

def ReferenceType(DigestMethod:dict[str, Any], DigestValue:bytearray, Id:str|None=None, Type_:str|None=None, URI:str|None=None, Transforms:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param DigestMethod: dict[DigestMethodType]
    :param DigestValue: bytearray
    :param Id: string (optional)
    :param Type_: string (optional)
    :param URI: string (optional)
    :param Transforms: dict[TransformsType] (optional)
    """

    DigestValue_bytes = list(DigestValue)
    j:dict[str, Any] = {
        "DigestMethod": DigestMethod,
        "DigestValue": {
            "bytesLen": len(DigestValue_bytes),
            "bytes": DigestValue_bytes
        }
    }

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    if Type_ is not None:
        Type__characters = [ord(x) for x in Type_]
        j["Type"] = {
            "charactersLen": len(Type__characters),
            "characters": Type__characters
        }

    if URI is not None:
        URI_characters = [ord(x) for x in URI]
        j["URI"] = {
            "charactersLen": len(URI_characters),
            "characters": URI_characters
        }

    if Transforms is not None:
        j["Transforms"] = Transforms
    
    return j

def RetrievalMethodType(Type:str|None=None, URI:str|None=None, Transforms:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param Type: string (optional)
    :param URI: string (optional)
    :param Transforms: dict[TransformsType] (optional)
    """

    j:dict[str, Any] = {}

    if Type is not None:
        Type_characters = [ord(x) for x in Type]
        j["Type"] = {
            "charactersLen": len(Type_characters),
            "characters": Type_characters
        }

    if URI is not None:
        URI_characters = [ord(x) for x in URI]
        j["URI"] = {
            "charactersLen": len(URI_characters),
            "characters": URI_characters
        }

    if Transforms is not None:
        j["Transforms"] = Transforms

    return j

def SalesTariffType(SalesTariffEntry:list[dict], SalesTariffID:int, Id:str|None=None, SalesTariffDescription:str|None=None, NumEPriceLevels:int|None=None)->dict[str, Any]:
    """
    :param SalesTariffEntry: list[dict[SalesTariffEntryType]]
    :param SalesTariffID: uint8/unsignedByte
    :param Id: string (optional)
    :param SalesTariffDescription: string (optional)
    :param NumEPriceLevels: uint8/unsignedByte (optional)
    """

    j:dict[str, Any] = {
        "SalesTariffEntry": {
            "arrayLen": len(SalesTariffEntry),
            "array": SalesTariffEntry
        },
        "SalesTariffID": SalesTariffID
    }

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    if SalesTariffDescription is not None:
        SalesTariffDescription_characters = [ord(x) for x in SalesTariffDescription]
        j["SalesTariffDescription"] = {
            "charactersLen": len(SalesTariffDescription_characters),
            "characters": SalesTariffDescription_characters
        }

    if NumEPriceLevels is not None:
        j["NumEPriceLevels"] = NumEPriceLevels

    return j

def X509DataType(X509IssuerSerial:dict[str, Any]|None=None, X509SKI:bytearray|None=None, X509SubjectName:str|None=None, X509Certificate:bytearray|None=None, X509CRL:bytearray|None=None, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param X509IssuerSerial: dict[X509IssuerSerialType] (optional)
    :param X509SKI: bytearray (optional)
    :param X509SubjectName: string (optional)
    :param X509Certificate: bytearray (optional)
    :param X509CRL: bytearray (optional)
    :param ANY: bytearray (optional)
    """
    
    j:dict[str, Any] = {}

    if X509IssuerSerial is not None:
        j["X509IssuerSerial"] = X509IssuerSerial

    if X509SKI is not None:
        X509SKI_bytes = list(X509SKI)
        j["X509SKI"] = {
            "bytesLen": len(X509SKI_bytes),
            "bytes": X509SKI_bytes
        }

    if X509SubjectName is not None:
        X509SubjectName_characters = [ord(x) for x in X509SubjectName]
        j["X509SubjectName"] = {
            "charactersLen": len(X509SubjectName_characters),
            "characters": X509SubjectName_characters
        }

    if X509Certificate is not None:
        X509Certificate_bytes = list(X509Certificate)
        j["X509Certificate"] = {
            "bytesLen": len(X509Certificate_bytes),
            "bytes": X509Certificate_bytes
        }

    if X509CRL is not None:
        X509CRL_bytes = list(X509CRL)
        j["X509CRL"] = {
            "bytesLen": len(X509CRL_bytes),
            "bytes": X509CRL_bytes
        }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytesLen": len(ANY_bytes),
            "bytes": ANY_bytes
        }

    return j

# TODO: Maybe better way to do this? Unions are hard :/
def PGPDataType(choice_1_PGPKeyID:bytearray|None=None, choice_1_PGPKeyPacket:bytearray|None=None, choice_1_ANY:bytearray|None=None, choice_2_PGPKeyPacket:bytearray|None=None, choice_2_ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param choice_1_PGPKeyID: bytearray (optional)
    :param choice_1_PGPKeyPacket: bytearray (optional)
    :param choice_1_ANY: bytearray (optional)
    :param choice_2_PGPKeyPacket: bytearray (optional)
    :param choice_2_ANY: bytearray (optional)
    """
    
    j:dict[str, Any] = {}
    if choice_1_PGPKeyID is not None:
        choice_1_PGPKeyID_bytes = list(choice_1_PGPKeyID)
        j["choice_1"]["PGPKeyID"] = {
            "bytesLen": len(choice_1_PGPKeyID_bytes),
            "bytes": choice_1_PGPKeyID_bytes
        }
    if choice_1_PGPKeyPacket is not None:
        choice_1_PGPKeyPacket_bytes = list(choice_1_PGPKeyPacket)
        j["choice_1"]["PGPKeyPacket"] = {
            "bytesLen": len(choice_1_PGPKeyPacket_bytes),
            "bytes": choice_1_PGPKeyPacket_bytes
        }
    if choice_1_ANY is not None:
        choice_1_ANY_bytes = list(choice_1_ANY)
        j["choice_1"]["ANY"] = {
            "bytesLen": len(choice_1_ANY_bytes),
            "bytes": choice_1_ANY_bytes
        }
    if choice_2_PGPKeyPacket is not None:
        choice_2_PGPKeyPacket_bytes = list(choice_2_PGPKeyPacket)
        j["choice_2"]["PGPKeyPacket"] = {
            "bytesLen": len(choice_2_PGPKeyPacket_bytes),
            "bytes": choice_2_PGPKeyPacket_bytes
        }
    if choice_2_ANY is not None:
        choice_2_ANY_bytes = list(choice_2_ANY)
        j["choice_2"]["ANY"] = {
            "bytesLen": len(choice_2_ANY_bytes),
            "bytes": choice_2_ANY_bytes
        }

    return j

def SPKIDataType(SPKISexp:bytearray, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param SPKISexp: bytearray
    :param ANY: bytearray (optional)
    """

    SPKISexp_bytes = list(SPKISexp)
    j:dict[str, Any] = {
        "SPKISexp": {
            "bytesLen": len(SPKISexp_bytes),
            "bytes": SPKISexp_bytes
        }
    }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytesLen": len(ANY_bytes),
            "bytes": ANY_bytes
        }

    return j

def SignedInfoType(CanonicalizationMethod:dict[str, Any], SignatureMethod:dict[str, Any], Reference:list[dict], Id:str|None=None)->dict[str, Any]:
    """
    :param CanonicalizationMethod: dict[CanonicalizationMethodType]
    :param SignatureMethod: dict[SignatureMethodType]
    :param Reference: list[dict[ReferenceType]]
    :param Id: string (optional)
    """

    j:dict[str, Any] = {
        "CanonicalizationMethod": CanonicalizationMethod,
        "SignatureMethod": SignatureMethod,
        "Reference": {
            "arrayLen": len(Reference),
            "array": Reference
        }
    }

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    return j

def ProfileEntryType(ChargingProfileEntryStart:int, ChargingProfileEntryMaxPower:dict[str, Any], ChargingProfileEntryMaxNumberOfPhasesInUse:int|None=None)->dict[str, Any]:
    """
    :param ChargingProfileEntryStart: uint32/unsignedInt
    :param ChargingProfileEntryMaxPower: dict[PhysicalValueType]
    :param ChargingProfileEntryMaxNumberOfPhasesInUse: uint8/unsignedByte (optional)
    """

    j:dict[str, Any] = {
        "ChargingProfileEntryStart": ChargingProfileEntryStart,
        "ChargingProfileEntryMaxPower": ChargingProfileEntryMaxPower
    }

    if ChargingProfileEntryMaxNumberOfPhasesInUse is not None:
        j["ChargingProfileEntryMaxNumberOfPhasesInUse"] = ChargingProfileEntryMaxNumberOfPhasesInUse

    return j

def DC_EVStatusType(EVReady:int, EVErrorCode:DC_EVErrorCodeType, EVRESSSOC:int)->dict[str, Any]:
    """
    :param EVReady: boolean
    :param EVErrorCode: DC_EVErrorCodeType
    :param EVRESSSOC: int8/byte
    """

    j:dict[str, Any] = {
        "EVReady": EVReady,
        "EVErrorCode": EVErrorCode.value,
        "EVRESSSOC": EVRESSSOC
    }

    return j

def ParameterSetType(ParameterSetID:int, Parameter:list[dict])->dict[str, Any]:
    """
    :param ParameterSetID: int16/short
    :param Parameter: list[dict[ParameterType]]
    """

    j:dict[str, Any] = {
        "ParameterSetID": ParameterSetID,
        "Parameter": {
            "arrayLen": len(Parameter),
            "array": Parameter
        }
    }

    return j

def SAScheduleTupleType(SAScheduleTupleID:int, PMaxSchedule:dict[str, Any], SalesTariff:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param SAScheduleTupleID: uint8/unsignedByte
    :param PMaxSchedule: dict[PMaxScheduleType]
    :param SalesTariff: dict[SalesTariffType] (optional)
    """

    j:dict[str, Any] = {
        "SAScheduleTupleID": SAScheduleTupleID,
        "PMaxSchedule": PMaxSchedule
    }

    if SalesTariff is not None:
        j["SalesTariff"] = SalesTariff

    return j

def SelectedServiceType(ServiceID:int, ParameterSetID:int|None=None)->dict[str, Any]:
    """
    :param ServiceID: uint16/unsignedShort
    :param ParameterSetID: int16/short (optional)
    """

    j:dict[str, Any] = {
        "ServiceID": ServiceID
    }

    if ParameterSetID is not None:
        j["ParameterSetID"] = ParameterSetID

    return j

def ServiceType(ServiceID:int, ServiceCategory:serviceCategoryType, FreeService:int, ServiceName:str|None=None, ServiceScope:str|None=None)->dict[str, Any]:
    """
    :param ServiceID: uint16/unsignedShort
    :param ServiceCategory: serviceCategoryType
    :param FreeService: boolean
    :param ServiceName: string (optional)
    :param ServiceScope: string (optional)
    """

    j:dict[str, Any] = {
        "ServiceID": ServiceID,
        "ServiceCategory": ServiceCategory.value,
        "FreeService": FreeService
    }

    if ServiceName is not None:
        ServiceName_characters = [ord(x) for x in ServiceName]
        j["ServiceName"] = {
            "charactersLen": len(ServiceName_characters),
            "characters": ServiceName_characters
        }

    if ServiceScope is not None:
        ServiceScope_characters = [ord(x) for x in ServiceScope]
        j["ServiceScope"] = {
            "charactersLen": len(ServiceScope_characters),
            "characters": ServiceScope_characters
        }

    return j

def SignatureValueType(CONTENT:bytearray, Id:str|None=None)->dict[str, Any]:
    """
    :param CONTENT: bytearray
    :param Id: string (optional)
    """

    CONTENT_bytes = list(CONTENT)
    j:dict[str, Any] = {
        "CONTENT": {
            "bytesLen": len(CONTENT_bytes),
            "bytes": CONTENT_bytes
        }
    }

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    return j

def SubCertificatesType(Certificate:list[list[bytearray]])->dict[str, Any]:
    """
    :param Certificate: list[list[bytearray]]
    """

    j:dict[str, Any] = {
        "Certificate": {
            "arrayLen": len(Certificate),
            "array": [{
                    "byteslen": len(list(subCertificate)),
                    "bytes": list(subCertificate)
                } for subCertificate in Certificate]
        }
    }

    return j

def KeyInfoType(Id:str|None=None, KeyName:str|None=None, KeyValue:dict[str, Any]|None=None, RetrievalMethod:dict[str, Any]|None=None, X509Data:dict[str, Any]|None=None, PGPData:dict[str, Any]|None=None, SPKIData:dict[str, Any]|None=None, MgmtData:str|None=None, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param Id: string (optional)
    :param KeyName: string (optional)
    :param KeyValue: dict[KeyValueType] (optional)
    :param RetrievalMethod: dict[RetrievalMethodType] (optional)
    :param X509Data: dict[X509DataType] (optional)
    :param PGPData: dict[PGPDataType] (optional)
    :param SPKIData: dict[SPKIDataType] (optional)
    :param MgmtData: string (optional)
    :param ANY: bytearray (optional)
    """    

    j:dict[str, Any] = {}

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    if KeyName is not None:
        KeyName_characters = [ord(x) for x in KeyName]
        j["KeyName"] = {
            "charactersLen": len(KeyName_characters),
            "characters": KeyName_characters
        }

    if KeyValue is not None:
        j["KeyValue"] = KeyValue

    if RetrievalMethod is not None:
        j["RetrievalMethod"] = RetrievalMethod

    if X509Data is not None:
        j["X509Data"] = X509Data

    if PGPData is not None:
        j["PGPData"] = PGPData

    if SPKIData is not None:
        j["SPKIData"] = SPKIData

    if MgmtData is not None:
        MgmtData_characters = [ord(x) for x in MgmtData]
        j["MgmtData"] = {
            "charactersLen": len(MgmtData_characters),
            "characters": MgmtData_characters
        }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytesLen": len(ANY_bytes),
            "bytes": ANY_bytes
        }

    return j

def ObjectType(Encoding:str|None=None, Id:str|None=None, MimeType:str|None=None, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param Encoding: string (optional)
    :param Id: string (optional)
    :param MimeType: string (optional)
    :param ANY: bytearray (optional)
    """

    j:dict[str, Any] = {}

    if Encoding is not None:
        Encoding_characters = [ord(x) for x in Encoding]
        j["Encoding"] = {
            "charactersLen": len(Encoding_characters),
            "characters": Encoding_characters
        }

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    if MimeType is not None:
        MimeType_characters = [ord(x) for x in MimeType]
        j["MimeType"] = {
            "charactersLen": len(MimeType_characters),
            "characters": MimeType_characters
        }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytesLen": len(ANY_bytes),
            "bytes": ANY_bytes
        }

    return j

def SupportedEnergyTransferModeType(EnergyTransferMode:list[EnergyTransferModeType])->dict[str, Any]:
    """
    :param EnergyTransferMode: list[EnergyTransferModeType]
    """

    j:dict[str, Any] = {
        "EnergyTransferMode": {
            "arrayLen": len(EnergyTransferMode),
            "array": [etm.value for etm in EnergyTransferMode]
        }
    }

    return j

def CertificateChainType(Certificate:bytearray, Id:str|None=None, SubCertificates:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param Certificate: bytearray
    :param Id: string (optional)
    :param SubCertificates: dict[SubCertificatesType] (optional)
    """

    Certificate_bytes = list(Certificate)
    j:dict[str, Any] = {
        "Certificate": {
            "bytesLen": len(Certificate_bytes),
            "bytes": Certificate_bytes
        }
    }

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    if SubCertificates is not None:
        # SubCertificatesType
        j["SubCertificates"] = SubCertificates

    return j

def BodyBaseType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str, Any] = {
        "_unused": _unused
    }
    return j

def NotificationType(FaultCode:faultCodeType, FaultMsg:str|None=None)->dict[str, Any]:
    """
    :param FaultCode: faultCodeType
    :param FaultMsg: string (optional)
    """

    j:dict[str, Any] = {
        "FaultCode": FaultCode.value
    }

    if FaultMsg is not None:
        FaultMsg_characters = [ord(x) for x in FaultMsg]
        j["FaultMsg"] = {
            "charactersLen": len(FaultMsg_characters),
            "characters": FaultMsg_characters
        }

    return j

def DC_EVSEStatusType(NotificationMaxDelay:int, EVSENotification:EVSENotificationType, EVSEStatusCode:DC_EVSEStatusCodeType, EVSEIsolationStatus:isolationLevelType|None=None)->dict[str, Any]:
    """
    :param NotificationMaxDelay: uint16/unsignedShort
    :param EVSENotification: EVSENotificationType
    :param EVSEStatusCode: DC_EVSEStatusCodeType
    :param EVSEIsolationStatus: isolationLevelType (optional)
    """
    
    j:dict[str, Any] = {
        "NotificationMaxDelay": NotificationMaxDelay,
        "EVSENotification": EVSENotification.value,
        "EVSEStatusCode": EVSEStatusCode.value
    }

    if EVSEIsolationStatus is not None:
        j["EVSEIsolationStatus"] = EVSEIsolationStatus.value

    return j

def SelectedServiceListType(SelectedService:list[dict])->dict[str, Any]:
    """
    :param SelectedService: list[dict[SelectedServiceType]]
    """

    j:dict[str, Any] = {
        "SelectedService": {
            "arrayLen": len(SelectedService),
            "array": SelectedService
        }
    }

    return j

def PaymentOptionListType(PaymentOption:list[paymentOptionType])->dict[str, Any]:
    """
    :param PaymentOption: list[paymentOptionType]
    """

    j:dict[str, Any] = {
        "PaymentOption": {
            "arrayLen": len(PaymentOption),
            "array": [po.value for po in PaymentOption]
        }
    }

    return j

def SignatureType(SignedInfo:dict[str, Any], SignatureValue:dict[str, Any], KeyInfo:dict[str, Any]|None=None, Id:str|None=None, Object:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param SignedInfo: dict[SignedInfoType]
    :param SignatureValue: dict[SignatureValueType]
    :param KeyInfo: dict[KeyInfoType] (optional)
    :param Id: string (optional)
    :param Object: dict[ObjectType] (optional)
    """

    j:dict[str, Any] = {
        "SignedInfo": SignedInfo,
        "SignatureValue": SignatureValue
    }

    if KeyInfo is not None:
        j["KeyInfo"] = KeyInfo

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    if Object is not None:
        j["Object"] = Object

    return j

def ChargingProfileType(ProfileEntry:list[dict])->dict[str, Any]:
    """
    :param ProfileEntry: list[dict[ProfileEntryType]]
    """

    j:dict[str, Any] = {
        "ProfileEntry": {
            "arrayLen": len(ProfileEntry),
            "array": ProfileEntry
        }
    }

    return j

def ServiceParameterListType(ParameterType:list[dict])->dict[str, Any]:
    """
    :param ParameterType: list[dict[ParameterType]]
    """

    j:dict[str, Any] = {
        "ParameterType": {
            "arrayLen": len(ParameterType),
            "array": ParameterType
        }
    }

    return j

def ListOfRootCertificateIDsType(RootCertificateID:list[dict])->dict[str, Any]:
    """
    :param RootCertificateID: list[dict[X509IssuerSerialType]]
    """

    j:dict[str, Any] = {
        "RootCertificateID": {
            "arrayLen": len(RootCertificateID),
            "array": RootCertificateID
        }
    }

    return j

def AC_EVChargeParameterType(EAmount:dict[str, Any], EVMaxVoltage:dict[str, Any], EVMaxCurrent:dict[str, Any], EVMinCurrent:dict[str, Any], DepartureTime:int|None=None)->dict[str, Any]:
    """
    :param EAmount: dict[PhysicalValueType]
    :param EVMaxVoltage: dict[PhysicalValueType]
    :param EVMaxCurrent: dict[PhysicalValueType]
    :param EVMinCurrent: dict[PhysicalValueType]
    :param DepartureTime: uint32/unsignedInt (optional)
    """

    j:dict[str, Any] = {
        "EAmount": EAmount,
        "EVMaxVoltage": EVMaxVoltage,
        "EVMaxCurrent": EVMaxCurrent,
        "EVMinCurrent": EVMinCurrent
    }

    if DepartureTime is not None:
        j["DepartureTime"] = DepartureTime

    return j

def DC_EVChargeParameterType(DC_EVStatus:dict[str, Any], EVMaximumCurrentLimit:dict[str, Any], EVMaximumVoltageLimit:dict[str, Any], DepartureTime:int|None=None, EVMaximumPowerLimit:dict[str, Any]|None=None, EVEnergyCapacity:dict[str, Any]|None=None, EVEnergyRequest:dict[str, Any]|None=None, FullSOC:int|None=None, BulkSOC:int|None=None)->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    :param EVMaximumCurrentLimit: dict[PhysicalValueType]
    :param EVMaximumVoltageLimit: dict[PhysicalValueType]
    :param DepartureTime: uint32/unsignedInt (optional)
    :param EVMaximumPowerLimit: dict[PhysicalValueType] (optional)
    :param EVEnergyCapacity: dict[PhysicalValueType] (optional)
    :param EVEnergyRequest: dict[PhysicalValueType] (optional)
    :param FullSOC: int8/byte (optional)
    :param BulkSOC: int8/byte (optional)
    """
    
    j:dict[str, Any] = {
        "DC_EVStatus": DC_EVStatus,
        "EVMaximumCurrentLimit": EVMaximumCurrentLimit,
        "EVMaximumVoltageLimit": EVMaximumVoltageLimit
    }

    if DepartureTime is not None:
        j["DepartureTime"] = DepartureTime

    if EVMaximumPowerLimit is not None:
        j["EVMaximumPowerLimit"] = EVMaximumPowerLimit

    if EVEnergyCapacity is not None:
        j["EVEnergyCapacity"] = EVEnergyCapacity

    if EVEnergyRequest is not None:
        j["EVEnergyRequest"] = EVEnergyRequest

    if FullSOC is not None:
        j["FullSOC"] = FullSOC

    if BulkSOC is not None:
        j["BulkSOC"] = BulkSOC

    return j

def EVChargeParameterType(AC_EVChargeParameter:dict[str, Any], DC_EVChargeParameter:dict[str, Any], DepartureTime:int|None=None)->dict[str, Any]:
    """
    :param AC_EVChargeParameter: dict[AC_EVChargeParameterType]
    :param DC_EVChargeParameter: dict[DC_EVChargeParameterType]
    :param DepartureTime: uint32/unsignedInt (optional)
    """

    j:dict[str, Any] = {
        "AC_EVChargeParameter": AC_EVChargeParameter,
        "DC_EVChargeParameter": DC_EVChargeParameter
    }

    if DepartureTime is not None:
        j["DepartureTime"] = DepartureTime

    return j

def SASchedulesType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """
    
    j:dict[str, Any] = {
        "_unused": _unused
    }
    return j

def SAScheduleListType(SAScheduleTuple:list[dict])->dict[str, Any]:
    """
    :param SAScheduleTuple: list[dict[SAScheduleTupleType]]
    """

    j:dict[str, Any] = {
        "SAScheduleTuple": {
            "arrayLen": len(SAScheduleTuple),
            "array": SAScheduleTuple
        }
    }

    return j

def ChargeServiceType(ServiceID:int, ServiceCategory:serviceCategoryType, FreeService:int, SupportedEnergyTransferMode:dict[str, Any], ServiceName:str|None=None, ServiceScope:str|None=None)->dict[str, Any]:
    """
    :param ServiceID: uint16/unsignedShort
    :param ServiceCategory: serviceCategoryType
    :param FreeService: boolean
    :param SupportedEnergyTransferMode: dict[SupportedEnergyTransferModeType]
    :param ServiceName: string (optional)
    :param ServiceScope: string (optional)
    """

    j:dict[str, Any] = {
        "ServiceID": ServiceID,
        "ServiceCategory": ServiceCategory.value,
        "FreeService": FreeService,
        "SupportedEnergyTransferMode": SupportedEnergyTransferMode
    }

    if ServiceName is not None:
        ServiceName_characters = [ord(x) for x in ServiceName]
        j["ServiceName"] = {
            "charactersLen": len(ServiceName_characters),
            "characters": ServiceName_characters
        }

    if ServiceScope is not None:
        ServiceScope_characters = [ord(x) for x in ServiceScope]
        j["ServiceScope"] = {
            "charactersLen": len(ServiceScope_characters),
            "characters": ServiceScope_characters
        }

    return j

def EVPowerDeliveryParameterType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str, Any] = {
        "_unused": _unused
    }
    return j

def DC_EVPowerDeliveryParameterType(DC_EVSEStatus:dict[str, Any], ChargingComplete:int, BulkChargingComplete:int|None=None)->dict[str, Any]:
    """
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param ChargingComplete: boolean
    :param BulkChargingComplete: boolean (optional)
    """

    j:dict[str, Any] = {
        "DC_EVSEStatus": DC_EVSEStatus,
        "ChargingComplete": ChargingComplete
    }

    if BulkChargingComplete is not None:
        j["BulkChargingComplete"] = BulkChargingComplete

    return j

def ContractSignatureEncryptedPrivateKeyType(Id:str, CONTENT:bytearray)->dict[str, Any]:
    """
    :param Id: string
    :param CONTENT: bytearray
    """

    CONTENT_bytes = list(CONTENT)
    Id_characters = [ord(x) for x in Id]
    j:dict[str, Any] = {
        "Id": {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        },
        "CONTENT": {
            "bytesLen": len(CONTENT_bytes),
            "bytes": CONTENT_bytes
        }
    }

    return j

def EVSEChargeParameterType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str, Any] = {
        "_unused": _unused
    }
    return j

def DC_EVSEChargeParameterType(DC_EVSEStatus:dict[str, Any], EVSEMaximumCurrentLimit:dict[str, Any], EVSEMaximumPowerLimit:dict[str, Any], EVSEMaximumVoltageLimit:dict[str, Any], EVSEMinimumCurrentLimit:dict[str, Any], EVSEMinimumVoltageLimit:dict[str, Any], EVSEPeakCurrentRipple:dict[str, Any], EVSECurrentRegulationTolerance:dict[str, Any]|None=None, EVSEEnergyToBeDelivered:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEMaximumCurrentLimit: dict[PhysicalValueType]
    :param EVSEMaximumPowerLimit: dict[PhysicalValueType]
    :param EVSEMaximumVoltageLimit: dict[PhysicalValueType]
    :param EVSEMinimumCurrentLimit: dict[PhysicalValueType]
    :param EVSEMinimumVoltageLimit: dict[PhysicalValueType]
    :param EVSEPeakCurrentRipple: dict[PhysicalValueType]
    :param EVSECurrentRegulationTolerance: dict[PhysicalValueType] (optional)
    :param EVSEEnergyToBeDelivered: dict[PhysicalValueType] (optional)
    """
    
    j:dict[str, Any] = {
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEMaximumCurrentLimit": EVSEMaximumCurrentLimit,
        "EVSEMaximumPowerLimit": EVSEMaximumPowerLimit,
        "EVSEMaximumVoltageLimit": EVSEMaximumVoltageLimit,
        "EVSEMinimumCurrentLimit": EVSEMinimumCurrentLimit,
        "EVSEMinimumVoltageLimit": EVSEMinimumVoltageLimit,
        "EVSEPeakCurrentRipple": EVSEPeakCurrentRipple
    }

    if EVSECurrentRegulationTolerance is not None:
        j["EVSECurrentRegulationTolerance"] = EVSECurrentRegulationTolerance

    if EVSEEnergyToBeDelivered is not None:
        j["EVSEEnergyToBeDelivered"] = EVSEEnergyToBeDelivered

    return j

def ServiceListType(Service:list[dict])->dict[str, Any]:
    """
    :param Service: list[dict[ServiceType]]
    """
    
    j:dict[str, Any] = {
        "Service": {
            "arrayLen": len(Service),
            "array": Service
        }
    }

    return j

def DiffieHellmanPublickeyType(Id:str, CONTENT:bytearray)->dict[str, Any]:
    """
    :param Id: string
    :param CONTENT: bytearray
    """

    CONTENT_bytes = list(CONTENT)
    Id_characters = [ord(x) for x in Id]
    j:dict[str, Any] = {
        "Id": {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        },
        "CONTENT": {
            "bytesLen": len(CONTENT_bytes),
            "bytes": CONTENT_bytes
        }
    }

    return j

def EMAIDType(Id:str, CONTENT:bytearray)->dict[str, Any]:
    """
    :param Id: string
    :param CONTENT: bytearray
    """

    CONTENT_bytes = list(CONTENT)
    Id_characters = [ord(x) for x in Id]
    j:dict[str, Any] = {
        "Id": {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        },
        "CONTENT": {
            "bytesLen": len(CONTENT_bytes),
            "bytes": CONTENT_bytes
        }
    }

    return j

def AC_EVSEStatusType(NotificationMaxDelay:int, EVSENotification:EVSENotificationType, RCD:int)->dict[str, Any]:
    """
    :param NotificationMaxDelay: uint16/unsignedShort
    :param EVSENotification: EVSENotificationType
    :param RCD: boolean
    """

    j:dict[str, Any] = {
        "NotificationMaxDelay": NotificationMaxDelay,
        "EVSENotification": EVSENotification.value,
        "RCD": RCD
    }

    return j

def EVSEStatusType(NotificationMaxDelay:int, EVSENotification:EVSENotificationType, AC_EVSEStatus:dict[str, Any], DC_EVSEStatus:dict[str, Any])->dict[str, Any]:
    """
    :param NotificationMaxDelay: uint16/unsignedShort
    :param EVSENotification: EVSENotificationType
    :param AC_EVSEStatus: dict[AC_EVSEStatusType]
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    """

    j:dict[str, Any] = {
        "NotificationMaxDelay": NotificationMaxDelay,
        "EVSENotification": EVSENotification.value,
        "AC_EVSEStatus": AC_EVSEStatus,
        "DC_EVSEStatus": DC_EVSEStatus
    }

    return j

def AC_EVSEChargeParameterType(AC_EVSEStatus:dict[str, Any], EVSENominalVoltage:dict[str, Any], EVSEMaxCurrent:dict[str, Any])->dict[str, Any]:
    """
    :param AC_EVSEStatus: dict[AC_EVSEStatusType]
    :param EVSENominalVoltage: dict[PhysicalValueType]
    :param EVSEMaxCurrent: dict[PhysicalValueType]
    """

    j:dict[str, Any] = {
        "AC_EVSEStatus": AC_EVSEStatus,
        "EVSENominalVoltage": EVSENominalVoltage,
        "EVSEMaxCurrent": EVSEMaxCurrent
    }

    return j

def MeterInfoType(MeterID:str, MeterReading:int|None=None, SigMeterReading:bytearray|None=None, MeterStatus:int|None=None, TMeter:int|None=None)->dict[str, Any]:
    """
    :param MeterID: string
    :param MeterReading: uint64/unsignedLong (optional)
    :param SigMeterReading: bytearray (optional)
    :param MeterStatus: int16/short (optional)
    :param TMeter: int64/long (optional)
    """

    j:dict[str, Any] = {
        "MeterID": MeterID
    }

    if MeterReading is not None:
        j["MeterReading"] = MeterReading

    if SigMeterReading is not None:
        SigMeterReading_bytes = list(SigMeterReading)
        j["SigMeterReading"] = {
            "bytesLen": len(SigMeterReading_bytes),
            "bytes": SigMeterReading_bytes
        }

    if MeterStatus is not None:
        j["MeterStatus"] = MeterStatus

    if TMeter is not None:
        j["TMeter"] = TMeter

    return j

def MessageHeaderType(SessionID:bytearray, Notification:dict[str, Any]|None=None, Signature:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param SessionID: bytearray
    :param Notification: dict[NotificationType] (optional)
    :param Signature: dict[SignatureType] (optional)
    """

    SessionID_bytes = list(SessionID)
    j:dict[str, Any] = {
        "SessionID": {
            "bytesLen": len(SessionID_bytes),
            "bytes": SessionID_bytes
        }
    }

    if Notification is not None:
        j["Notification"] = Notification

    if Signature is not None:
        j["Signature"] = Signature

    return j

def PowerDeliveryReqType(ChargeProgress:chargeProgressType, SAScheduleTupleID:int, ChargingProfile:dict[str, Any]|None=None, DC_EVPowerDeliveryParameter:dict[str, Any]|None=None, EVPowerDeliveryParameter:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ChargeProgress: chargeProgressType
    :param SAScheduleTupleID: uint8/unsignedByte
    :param ChargingProfile: dict[ChargingProfileType] (optional)
    :param DC_EVPowerDeliveryParameter: dict[DC_EVPowerDeliveryParameterType] (optional)
    :param EVPowerDeliveryParameter: dict[EVPowerDeliveryParameterType] (optional)
    """
    
    j:dict[str, Any] = {
        "ChargeProgress": ChargeProgress.value,
        "SAScheduleTupleID": SAScheduleTupleID
    }

    if ChargingProfile is not None:
        j["ChargingProfile"] = ChargingProfile

    if DC_EVPowerDeliveryParameter is not None:
        j["DC_EVPowerDeliveryParameter"] = DC_EVPowerDeliveryParameter

    if EVPowerDeliveryParameter is not None:
        j["EVPowerDeliveryParameter"] = EVPowerDeliveryParameter

    return j

def CurrentDemandResType(ResponseCode:responseCodeType, DC_EVSEStatus:dict[str, Any], EVSEPresentVoltage:dict[str, Any], EVSEPresentCurrent:dict[str, Any], EVSECurrentLimitAchieved:int, EVSEVoltageLimitAchieved:int, EVSEPowerLimitAchieved:int, EVSEID:str, SAScheduleTupleID:int, EVSEMaximumVoltageLimit:dict[str, Any]|None=None, EVSEMaximumCurrentLimit:dict[str, Any]|None=None, EVSEMaxiumPowerLimit:dict[str, Any]|None=None, MeterInfo:dict[str, Any]|None=None, ReceiptRequired:int|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEPresentVoltage: dict[PhysicalValueType]
    :param EVSEPresentCurrent: dict[PhysicalValueType]
    :param EVSECurrentLimitAchieved: boolean
    :param EVSEVoltageLimitAchieved: boolean
    :param EVSEPowerLimitAchieved: boolean
    :param EVSEID: string
    :param SAScheduleTupleID: uint8/unsignedByte
    :param EVSEMaximumVoltageLimit: dict[PhysicalValueType] (optional)
    :param EVSEMaximumCurrentLimit: dict[PhysicalValueType] (optional)
    :param EVSEMaxiumPowerLimit: dict[PhysicalValueType] (optional)
    :param MeterInfo: dict[MeterInfoType] (optional)
    :param ReceiptRequired: boolean (optional)
    """
    
    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEPresentVoltage": EVSEPresentVoltage,
        "EVSEPresentCurrent": EVSEPresentCurrent,
        "EVSECurrentLimitAchieved": EVSECurrentLimitAchieved,
        "EVSEVoltageLimitAchieved": EVSEVoltageLimitAchieved,
        "EVSEPowerLimitAchieved": EVSEPowerLimitAchieved,
        "EVSEID": EVSEID,
        "SAScheduleTupleID": SAScheduleTupleID
    }

    if EVSEMaximumVoltageLimit is not None:
        j["EVSEMaximumVoltageLimit"] = EVSEMaximumVoltageLimit

    if EVSEMaximumCurrentLimit is not None:
        j["EVSEMaximumCurrentLimit"] = EVSEMaximumCurrentLimit

    if EVSEMaxiumPowerLimit is not None:
        j["EVSEMaxiumPowerLimit"] = EVSEMaxiumPowerLimit

    if MeterInfo is not None:
        j["MeterInfo"] = MeterInfo

    if ReceiptRequired is not None:
        j["ReceiptRequired"] = ReceiptRequired

    return j

def ChargingStatusResType(ResponseCode:responseCodeType, EVSEID:str, SAScheduleTupleID:int, AC_EVSEStatus:dict[str, Any], EVSEMaxCurrent:dict[str, Any]|None=None, MeterInfo:dict[str, Any]|None=None, ReceiptRequired:int|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param EVSEID: string
    :param SAScheduleTupleID: uint8/unsignedByte
    :param AC_EVSEStatus: dict[AC_EVSEStatusType]
    :param EVSEMaxCurrent: dict[PhysicalValueType] (optional)
    :param MeterInfo: dict[MeterInfoType] (optional)
    :param ReceiptRequired: boolean (optional)
    """
    
    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "EVSEID": EVSEID,
        "SAScheduleTupleID": SAScheduleTupleID,
        "AC_EVSEStatus": AC_EVSEStatus
    }

    if EVSEMaxCurrent is not None:
        j["EVSEMaxCurrent"] = EVSEMaxCurrent

    if MeterInfo is not None:
        j["MeterInfo"] = MeterInfo

    if ReceiptRequired is not None:
        j["ReceiptRequired"] = ReceiptRequired

    return j

def AuthorizationReqType(Id:str|None=None, GenChallenge:bytearray|None=None)->dict[str, Any]:
    """
    :param Id: string (optional)
    :param GenChallenge: bytearray (optional)
    """

    j:dict[str, Any] = {}

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    if GenChallenge is not None:
        GenChallenge_bytes = list(GenChallenge)
        j["GenChallenge"] = {
            "bytesLen": len(GenChallenge_bytes),
            "bytes": GenChallenge_bytes
        }

    return j

def PreChargeReqType(DC_EVStatus:dict[str, Any], EVTargetVoltage:dict[str, Any], EVTargetCurrent:dict[str, Any])->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    :param EVTargetVoltage: dict[PhysicalValueType]
    :param EVTargetCurrent: dict[PhysicalValueType]
    """

    j:dict[str, Any] = {
        "DC_EVStatus": DC_EVStatus,
        "EVTargetVoltage": EVTargetVoltage,
        "EVTargetCurrent": EVTargetCurrent
    }

    return j

def ServiceDetailResType(ResponseCode:responseCodeType, ServiceID:int, ServiceParameterList:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param ServiceID: uint16/unsignedShort
    :param ServiceParameterList: dict[ServiceParameterListType] (optional)
    """

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "ServiceID": ServiceID
    }

    if ServiceParameterList is not None:
        j["ServiceParameterList"] = ServiceParameterList

    return j

def PaymentServiceSelectionResType(ResponseCode:responseCodeType)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    """

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value
    }

    return j

def CertificateUpdateReqType(Id:str, ContractSignatureCertChain:dict[str, Any], eMAID:dict[str, Any], ListOfRootCertificateIDs:dict[str, Any])->dict[str, Any]:
    """
    :param Id: string
    :param ContractSignatureCertChain: dict[CertificateChainType]
    :param eMAID: dict[EMAIDType]
    :param ListOfRootCertificateIDs: dict[ListOfRootCertificateIDsType]
    """

    j:dict[str, Any] = {
        "Id": Id,
        "ContractSignatureCertChain": ContractSignatureCertChain,
        "eMAID": eMAID,
        "ListOfRootCertificateIDs": ListOfRootCertificateIDs
    }

    return j

def SessionSetupResType(ResponseCode:responseCodeType, EVSEID:str, EVSETimeStamp:int|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param EVSEID: string
    :param EVSETimeStamp: int64/long (optional)
    """

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "EVSEID": EVSEID
    }

    if EVSETimeStamp is not None:
        j["EVSETimeStamp"] = EVSETimeStamp

    return j

def CertificateInstallationReqType(Id:str, OEMProvisioningCert:bytearray, ListOfRootCertificateIDs:dict[str, Any])->dict[str, Any]:
    """
    :param Id: string
    :param OEMProvisioningCert: bytearray
    :param ListOfRootCertificateIDs: dict[ListOfRootCertificateIDsType]
    """

    OEMProvisioningCert_bytes = list(OEMProvisioningCert)
    j:dict[str, Any] = {
        "Id": Id,
        "OEMProvisioningCert": {
            "bytesLen": len(OEMProvisioningCert_bytes),
            "bytes": OEMProvisioningCert_bytes
        },
        "ListOfRootCertificateIDs": ListOfRootCertificateIDs
    }

    return j

def CertificateInstallationResType(ResponseCode:responseCodeType, SAProvisioningCertChain:dict[str, Any], ContractSignatureCertChain:dict[str, Any], ContractSignatureEncryptedPrivateKey:dict[str, Any], DHpublickey:dict[str, Any], eMAID:dict[str, Any])->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param SAProvisioningCertChain: dict[CertificateChainType]
    :param ContractSignatureCertChain: dict[CertificateChainType]
    :param ContractSignatureEncryptedPrivateKey: dict[ContractSignatureEncryptedPrivateKeyType]
    :param DHpublickey: dict[DiffieHellmanPublickeyType]
    :param eMAID: dict[EMAIDType]
    """
    
    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "SAProvisioningCertChain": SAProvisioningCertChain,
        "ContractSignatureCertChain": ContractSignatureCertChain,
        "ContractSignatureEncryptedPrivateKey": ContractSignatureEncryptedPrivateKey,
        "DHpublickey": DHpublickey,
        "eMAID": eMAID
    }

    return j

def WeldingDetectionResType(ResponseCode:responseCodeType, DC_EVSEStatus:dict[str, Any], EVSEPresentVoltage:dict[str, Any])->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEPresentVoltage: dict[PhysicalValueType]
    """

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEPresentVoltage": EVSEPresentVoltage
    }

    return j

def CurrentDemandReqType(DC_EVStatus:dict[str, Any], EVTargetCurrent:dict[str, Any], ChargingComplete:int, EVTargetVoltage:dict[str, Any], EVMaximumVoltageLimit:dict[str, Any]|None=None, EVMaximumCurrentLimit:dict[str, Any]|None=None, EVMaximumPowerLimit:dict[str, Any]|None=None, BulkChargingComplete:int|None=None, RemainingTimeToFullSOC:dict[str, Any]|None=None, RemainingTimeToBulkSOC:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    :param EVTargetCurrent: dict[PhysicalValueType]
    :param ChargingComplete: boolean
    :param EVTargetVoltage: dict[PhysicalValueType]
    :param EVMaximumVoltageLimit: dict[PhysicalValueType] (optional)
    :param EVMaximumCurrentLimit: dict[PhysicalValueType] (optional)
    :param EVMaximumPowerLimit: dict[PhysicalValueType] (optional)
    :param BulkChargingComplete: boolean (optional)
    :param RemainingTimeToFullSOC: dict[PhysicalValueType] (optional)
    :param RemainingTimeToBulkSOC: dict[PhysicalValueType] (optional)
    """
    
    j:dict[str, Any] = {
        "DC_EVStatus": DC_EVStatus,
        "EVTargetCurrent": EVTargetCurrent,
        "ChargingComplete": ChargingComplete,
        "EVTargetVoltage": EVTargetVoltage
    }

    if EVMaximumVoltageLimit is not None:
        j["EVMaximumVoltageLimit"] = EVMaximumVoltageLimit

    if EVMaximumCurrentLimit is not None:
        j["EVMaximumCurrentLimit"] = EVMaximumCurrentLimit

    if EVMaximumPowerLimit is not None:
        j["EVMaximumPowerLimit"] = EVMaximumPowerLimit

    if BulkChargingComplete is not None:
        j["BulkChargingComplete"] = BulkChargingComplete

    if RemainingTimeToFullSOC is not None:
        j["RemainingTimeToFullSOC"] = RemainingTimeToFullSOC

    if RemainingTimeToBulkSOC is not None:
        j["RemainingTimeToBulkSOC"] = RemainingTimeToBulkSOC

    return j

def PreChargeResType(ResponseCode:responseCodeType, DC_EVSEStatus:dict[str, Any], EVSEPresentVoltage:dict[str, Any])->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEPresentVoltage: dict[PhysicalValueType]
    """

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEPresentVoltage": EVSEPresentVoltage
    }

    return j

def CertificateUpdateResType(ResponseCode:responseCodeType, SAProvisioningCertChain:dict[str, Any], ContractSignatureCertChain:dict[str, Any], ContractSignatureEncryptedPrivateKey:dict[str, Any], DHpublickey:dict[str, Any], eMAID:dict[str, Any], RetryCounter:int|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param SAProvisioningCertChain: dict[CertificateChainType]
    :param ContractSignatureCertChain: dict[CertificateChainType]
    :param ContractSignatureEncryptedPrivateKey: dict[ContractSignatureEncryptedPrivateKeyType]
    :param DHpublickey: dict[DiffieHellmanPublickeyType]
    :param eMAID: dict[EMAIDType]
    :param RetryCounter: int16/byte (optional)
    """
    
    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "SAProvisioningCertChain": SAProvisioningCertChain,
        "ContractSignatureCertChain": ContractSignatureCertChain,
        "ContractSignatureEncryptedPrivateKey": ContractSignatureEncryptedPrivateKey,
        "DHpublickey": DHpublickey,
        "eMAID": eMAID
    }

    if RetryCounter is not None:
        j["RetryCounter"] = RetryCounter

    return j

def MeteringReceiptReqType(SessionID:bytearray, MeterInfo:dict[str, Any], Id:str|None=None, SAScheduleTupleID:int|None=None)->dict[str, Any]:
    """
    :param SessionID: bytearray
    :param MeterInfo: dict[MeterInfoType]
    :param Id: string (optional)
    :param SAScheduleTupleID: uint8/unsignedByte (optional)
    """
    
    SessionID_bytes = list(SessionID)
    j:dict[str, Any] = {
        "SessionID": {
            "bytesLen": len(SessionID_bytes),
            "bytes": SessionID_bytes
        },
        "MeterInfo": MeterInfo
    }

    if Id is not None:
        Id_characters = [ord(x) for x in Id]
        j["Id"] = {
            "charactersLen": len(Id_characters),
            "characters": Id_characters
        }

    if SAScheduleTupleID is not None:
        j["SAScheduleTupleID"] = SAScheduleTupleID

    return j

def ChargingStatusReqType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str, Any] = {
        "_unused": _unused
    }
    return j

def SessionStopResType(ResponseCode:responseCodeType)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    """

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value
    }

    return j

def ChargeParameterDiscoveryReqType(RequestedEnergyTransferMode:EnergyTransferModeType, MaxEntriesSAScheduleTuple:int|None=None, AC_EVChargeParameter:dict[str, Any]|None=None, DC_EVChargeParameter:dict[str, Any]|None=None, EVChargeParameter:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param RequestedEnergyTransferMode: EnergyTransferModeType
    :param MaxEntriesSAScheduleTuple: uint16/unsignedShort (optional)
    :param AC_EVChargeParameter: dict[AC_EVChargeParameterType] (optional)
    :param DC_EVChargeParameter: dict[DC_EVChargeParameterType] (optional)
    :param EVChargeParameter: dict[EVChargeParameterType] (optional)
    """
    
    j:dict[str, Any] = {
        "RequestedEnergyTransferMode": RequestedEnergyTransferMode.value
    }

    if MaxEntriesSAScheduleTuple is not None:
        j["MaxEntriesSAScheduleTuple"] = MaxEntriesSAScheduleTuple

    if AC_EVChargeParameter is not None:
        j["AC_EVChargeParameter"] = AC_EVChargeParameter

    if DC_EVChargeParameter is not None:
        j["DC_EVChargeParameter"] = DC_EVChargeParameter

    if EVChargeParameter is not None:
        j["EVChargeParameter"] = EVChargeParameter

    return j

def CableCheckReqType(DC_EVStatus:dict[str, Any])->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    """

    j:dict[str, Any] = {
        "DC_EVStatus": DC_EVStatus
    }

    return j

def WeldingDetectionReqType(DC_EVStatus:dict[str, Any])->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    """

    j:dict[str, Any] = {
        "DC_EVStatus": DC_EVStatus
    }

    return j

def PowerDeliveryResType(ResponseCode:responseCodeType, AC_EVSEStatus:dict[str, Any]|None=None, DC_EVSEStatus:dict[str, Any]|None=None, EVSEStatus:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param AC_EVSEStatus: dict[AC_EVSEStatusType] (optional)
    :param DC_EVSEStatus: dict[DC_EVSEStatusType] (optional)
    :param EVSEStatus: dict[EVSEStatusType] (optional)
    """
    
    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value
    }

    if AC_EVSEStatus is not None:
        j["AC_EVSEStatus"] = AC_EVSEStatus

    if DC_EVSEStatus is not None:
        j["DC_EVSEStatus"] = DC_EVSEStatus

    if EVSEStatus is not None:
        j["EVSEStatus"] = EVSEStatus

    return j

def ChargeParameterDiscoveryResType(ResponseCode:responseCodeType, EVSEProcessing:EVSEProcessingType, SAScheduleList:dict[str, Any]|None=None, SASchedules:dict[str, Any]|None=None, AC_EVSEChargeParameter:dict[str, Any]|None=None, DC_EVSEChargeParameter:dict[str, Any]|None=None, EVSEChargeParameter:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param EVSEProcessing: EVSEProcessingType
    :param SAScheduleList: dict[SAScheduleListType] (optional)
    :param SASchedules: dict[SASchedulesType] (optional)
    :param AC_EVSEChargeParameter: dict[AC_EVSEChargeParameterType] (optional)
    :param DC_EVSEChargeParameter: dict[DC_EVSEChargeParameterType] (optional)
    :param EVSEChargeParameter: dict[EVSEChargeParameterType] (optional)
    """
    
    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "EVSEProcessing": EVSEProcessing.value
    }

    if SAScheduleList is not None:
        j["SAScheduleList"] = SAScheduleList

    if SASchedules is not None:
        j["SASchedules"] = SASchedules

    if AC_EVSEChargeParameter is not None:
        j["AC_EVSEChargeParameter"] = AC_EVSEChargeParameter

    if DC_EVSEChargeParameter is not None:
        j["DC_EVSEChargeParameter"] = DC_EVSEChargeParameter

    if EVSEChargeParameter is not None:
        j["EVSEChargeParameter"] = EVSEChargeParameter

    return j

def PaymentServiceSelectionReqType(SelectedPaymentOption:paymentOptionType, SelectedServiceList:dict[str, Any])->dict[str, Any]:
    """
    :param SelectedPaymentOption: paymentOptionType
    :param SelectedServiceList: dict[ServiceListType]
    """

    j:dict[str, Any] = {
        "SelectedPaymentOption": SelectedPaymentOption.value,
        "SelectedServiceList": SelectedServiceList
    }
    return j

def MeteringReceiptResType(ResponseCode:responseCodeType, AC_EVSEStatus:dict[str, Any]|None=None, DC_EVSEStatus:dict[str, Any]|None=None, EVSEStatus:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param AC_EVSEStatus: dict[AC_EVSEStatusType] (optional)
    :param DC_EVSEStatus: dict[DC_EVSEStatusType] (optional)
    :param EVSEStatus: dict[EVSEStatusType] (optional)
    """
    
    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value
    }

    if AC_EVSEStatus is not None:
        j["AC_EVSEStatus"] = AC_EVSEStatus

    if DC_EVSEStatus is not None:
        j["DC_EVSEStatus"] = DC_EVSEStatus

    if EVSEStatus is not None:
        j["EVSEStatus"] = EVSEStatus

    return j

def CableCheckResType(ResponseCode:responseCodeType, DC_EVSEStatus:dict[str, Any], EVSEProcessing:EVSEProcessingType)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEProcessing: EVSEProcessingType
    """
    
    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEProcessing": EVSEProcessing.value
    }

    return j

def ServiceDetailReqType(ServiceID:int)->dict[str, Any]:
    """
    :param ServiceID: uint16/unsignedShort
    """
    
    j:dict[str, Any] = {
        "ServiceID": ServiceID
    }
    return j

def SessionSetupReqType(EVCCID:bytearray)->dict[str, Any]:
    """
    :param EVCCID: bytearray
    """

    EVCCID_bytes = list(EVCCID)
    j:dict[str, Any] = {
        "EVCCID": {
            "bytesLen": len(EVCCID_bytes),
            "bytes": EVCCID_bytes
        }
    }
    return j

def SessionStopReqType(ChargingSession:chargingSessionType)->dict[str, Any]:
    """
    :param ChargingSession: chargingSessionType
    """

    j:dict[str, Any] = {
        "ChargingSession": ChargingSession.value
    }
    return j

def ServiceDiscoveryReqType(ServiceScope:str|None=None, ServiceCategory:serviceCategoryType|None=None)->dict[str, Any]:
    """
    :param ServiceScope: str (optional)
    :param ServiceCategory: serviceCategoryType (optional)
    """

    j:dict[str, Any] = {}

    if ServiceScope is not None:
        ServiceScope_characters = [ord(x) for x in ServiceScope]
        j["ServiceScope"] = {
            "charactersLen": len(ServiceScope_characters),
            "characters": ServiceScope_characters
        }

    if ServiceCategory is not None:
        j["ServiceCategory"] = ServiceCategory.value

    return j

def AuthorizationResType(ResponseCode:responseCodeType, EVSEProcessing:EVSEProcessingType)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param EVSEProcessing: EVSEProcessingType
    """

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "EVSEProcessing": EVSEProcessing.value
    }
    return j

def PaymentDetailsReqType(eMAID:str, ContractSignatureCertChain:dict[str, Any])->dict[str, Any]:
    """
    :param eMAID: string
    :param ContractSignatureCertChain: dict[CertificateChainType]
    """

    eMAID_characters = [ord(x) for x in eMAID]
    j:dict[str, Any] = {
        "eMAID": {
            "charactersLen": len(eMAID_characters),
            "characters": eMAID_characters
        },
        "ContractSignatureCertChain": ContractSignatureCertChain
    }
    return j

def PaymentDetailsResType(ResponseCode:responseCodeType, GenChallenge:bytearray, EVSETimeStamp:int)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param GenChallenge: bytearray
    :param EVSETimeStamp: int64/long
    """

    GenChallenge_bytes = list(GenChallenge)
    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "GenChallenge": {
            "bytesLen": len(GenChallenge_bytes),
            "bytes": GenChallenge_bytes
        },
        "EVSETimeStamp": EVSETimeStamp
    }
    return j

def BodyType(AuthorizationReq:dict[str, Any]|None=None, AuthorizationRes:dict[str, Any]|None=None, BodyElement:dict[str, Any]|None=None, CableCheckReq:dict[str, Any]|None=None, CableCheckRes:dict[str, Any]|None=None, CertificateInstallationReq:dict[str, Any]|None=None, CertificateInstallationRes:dict[str, Any]|None=None, CertificateUpdateReq:dict[str, Any]|None=None, CertificateUpdateRes:dict[str, Any]|None=None, ChargeParameterDiscoveryReq:dict[str, Any]|None=None, ChargeParameterDiscoveryRes:dict[str, Any]|None=None, ChargingStatusReq:dict[str, Any]|None=None, ChargingStatusRes:dict[str, Any]|None=None, CurrentDemandReq:dict[str, Any]|None=None, CurrentDemandRes:dict[str, Any]|None=None, MeteringReceiptReq:dict[str, Any]|None=None, MeteringReceiptRes:dict[str, Any]|None=None, PaymentDetailsReq:dict[str, Any]|None=None, PaymentDetailsRes:dict[str, Any]|None=None, PaymentServiceSelectionReq:dict[str, Any]|None=None, PaymentServiceSelectionRes:dict[str, Any]|None=None, PowerDeliveryReq:dict[str, Any]|None=None, PowerDeliveryRes:dict[str, Any]|None=None, PreChargeReq:dict[str, Any]|None=None, PreChargeRes:dict[str, Any]|None=None, ServiceDetailReq:dict[str, Any]|None=None, ServiceDetailRes:dict[str, Any]|None=None, ServiceDiscoveryReq:dict[str, Any]|None=None, ServiceDiscoveryRes:dict[str, Any]|None=None, SessionSetupReq:dict[str, Any]|None=None, SessionSetupRes:dict[str, Any]|None=None, SessionStopReq:dict[str, Any]|None=None, SessionStopRes:dict[str, Any]|None=None, WeldingDetectionReq:dict[str, Any]|None=None, WeldingDetectionRes:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param AuthorizationReq: dict[AuthorizationReqType] (optional)
    :param AuthorizationRes: dict[AuthorizationResType] (optional)
    :param BodyElement: dict[BodyBaseType] (optional)
    :param CableCheckReq: dict[CableCheckReqType] (optional)
    :param CableCheckRes: dict[CableCheckResType] (optional)
    :param CertificateInstallationReq: dict[CertificateInstallationReqType] (optional)
    :param CertificateInstallationRes: dict[CertificateInstallationResType] (optional)
    :param CertificateUpdateReq: dict[CertificateUpdateReqType] (optional)
    :param CertificateUpdateRes: dict[CertificateUpdateResType] (optional)
    :param ChargeParameterDiscoveryReq: dict[ChargeParameterDiscoveryReqType] (optional)
    :param ChargeParameterDiscoveryRes: dict[ChargeParameterDiscoveryResType] (optional)
    :param ChargingStatusReq: dict[ChargingStatusReqType] (optional)
    :param ChargingStatusRes: dict[ChargingStatusResType] (optional)
    :param CurrentDemandReq: dict[CurrentDemandReqType] (optional)
    :param CurrentDemandRes: dict[CurrentDemandResType] (optional)
    :param MeteringReceiptReq: dict[MeteringReceiptReqType] (optional)
    :param MeteringReceiptRes: dict[MeteringReceiptResType] (optional)
    :param PaymentDetailsReq: dict[PaymentDetailsReqType] (optional)
    :param PaymentDetailsRes: dict[PaymentDetailsResType] (optional)
    :param PaymentServiceSelectionReq: dict[PaymentServiceSelectionReqType] (optional)
    :param PaymentServiceSelectionRes: dict[PaymentServiceSelectionResType] (optional)
    :param PowerDeliveryReq: dict[PowerDeliveryReqType] (optional)
    :param PowerDeliveryRes: dict[PowerDeliveryResType] (optional)
    :param PreChargeReq: dict[PreChargeReqType] (optional)
    :param PreChargeRes: dict[PreChargeResType] (optional)
    :param ServiceDetailReq: dict[ServiceDetailReqType] (optional)
    :param ServiceDetailRes: dict[ServiceDetailResType] (optional)
    :param ServiceDiscoveryReq: dict[ServiceDiscoveryReqType] (optional)
    :param ServiceDiscoveryRes: dict[ServiceDiscoveryResType] (optional)
    :param SessionSetupReq: dict[SessionSetupReqType] (optional)
    :param SessionSetupRes: dict[SessionSetupResType] (optional)
    :param SessionStopReq: dict[SessionStopReqType] (optional)
    :param SessionStopRes: dict[SessionStopResType] (optional)
    :param WeldingDetectionReq: dict[WeldingDetectionReqType] (optional)
    :param WeldingDetectionRes: dict[WeldingDetectionResType] (optional)
    """
    
    j:dict[str, Any] = {}

    if AuthorizationReq is not None:
        j["AuthorizationReq"] = AuthorizationReq

    if AuthorizationRes is not None:
        j["AuthorizationRes"] = AuthorizationRes

    if BodyElement is not None:
        j["BodyElement"] = BodyElement

    if CableCheckReq is not None:
        j["CableCheckReq"] = CableCheckReq

    if CableCheckRes is not None:
        j["CableCheckRes"] = CableCheckRes

    if CertificateInstallationReq is not None:
        j["CertificateInstallationReq"] = CertificateInstallationReq

    if CertificateInstallationRes is not None:
        j["CertificateInstallationRes"] = CertificateInstallationRes

    if CertificateUpdateReq is not None:
        j["CertificateUpdateReq"] = CertificateUpdateReq

    if CertificateUpdateRes is not None:
        j["CertificateUpdateRes"] = CertificateUpdateRes

    if ChargeParameterDiscoveryReq is not None:
        j["ChargeParameterDiscoveryReq"] = ChargeParameterDiscoveryReq

    if ChargeParameterDiscoveryRes is not None:
        j["ChargeParameterDiscoveryRes"] = ChargeParameterDiscoveryRes

    if ChargingStatusReq is not None:
        j["ChargingStatusReq"] = ChargingStatusReq

    if ChargingStatusRes is not None:
        j["ChargingStatusRes"] = ChargingStatusRes

    if CurrentDemandReq is not None:
        j["CurrentDemandReq"] = CurrentDemandReq

    if CurrentDemandRes is not None:
        j["CurrentDemandRes"] = CurrentDemandRes

    if MeteringReceiptReq is not None:
        j["MeteringReceiptReq"] = MeteringReceiptReq

    if MeteringReceiptRes is not None:
        j["MeteringReceiptRes"] = MeteringReceiptRes

    if PaymentDetailsReq is not None:
        j["PaymentDetailsReq"] = PaymentDetailsReq

    if PaymentDetailsRes is not None:
        j["PaymentDetailsRes"] = PaymentDetailsRes

    if PaymentServiceSelectionReq is not None:
        j["PaymentServiceSelectionReq"] = PaymentServiceSelectionReq

    if PaymentServiceSelectionRes is not None:
        j["PaymentServiceSelectionRes"] = PaymentServiceSelectionRes

    if PowerDeliveryReq is not None:
        j["PowerDeliveryReq"] = PowerDeliveryReq

    if PowerDeliveryRes is not None:
        j["PowerDeliveryRes"] = PowerDeliveryRes

    if PreChargeReq is not None:
        j["PreChargeReq"] = PreChargeReq

    if PreChargeRes is not None:
        j["PreChargeRes"] = PreChargeRes

    if ServiceDetailReq is not None:
        j["ServiceDetailReq"] = ServiceDetailReq
        
    if ServiceDetailRes is not None:
        j["ServiceDetailRes"] = ServiceDetailRes

    if ServiceDiscoveryReq is not None:
        j["ServiceDiscoveryReq"] = ServiceDiscoveryReq

    if ServiceDiscoveryRes is not None:
        j["ServiceDiscoveryRes"] = ServiceDiscoveryRes

    if SessionSetupReq is not None:
        j["SessionSetupReq"] = SessionSetupReq

    if SessionSetupRes is not None:
        j["SessionSetupRes"] = SessionSetupRes

    if SessionStopReq is not None:
        j["SessionStopReq"] = SessionStopReq

    if SessionStopRes is not None:
        j["SessionStopRes"] = SessionStopRes

    if WeldingDetectionReq is not None:
        j["WeldingDetectionReq"] = WeldingDetectionReq

    if WeldingDetectionRes is not None:
        j["WeldingDetectionRes"] = WeldingDetectionRes

    return j

def V2G_Message(Header:dict[str, Any], Body:dict[str, Any])->dict[str, Any]:
    """
    :param Header: dict[MessageHeaderType]
    :param Body: dict[BodyType]
    """

    j:dict[str, Any] = {
        "Header": Header,
        "Body": Body
    }
    
    return j