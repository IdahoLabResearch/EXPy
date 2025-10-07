"""
    Copyright 2025, Battelle Energy Alliance, LLC All Rights Reserved
"""


from enum import Enum
from typing import Any


class costKindType(Enum):
    relativePricePercentage = 0
    RenewableGenerationPercentage = 1
    CarbonDioxideEmission = 2


class isolationLevelType(Enum):
    Invalid = 0
    Valid = 1
    Warning = 2
    Fault = 3


class paymentOptionType(Enum):
    Contract = 0
    ExternalPayment = 1


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


class unitSymbolType(Enum):
    h = 0
    m = 1
    s = 2
    A = 3
    Ah = 4
    V = 5
    VA = 6
    W = 7
    W_s = 8
    Wh = 9


class EVSESupportedEnergyTransferType(Enum):
    AC_single_phase_core = 0
    AC_three_phase_core = 1
    DC_core = 2
    DC_extended = 3
    DC_combo_core = 4
    DC_dual = 5
    AC_core1p_DC_extended = 6
    AC_single_DC_core = 7
    AC_single_phase_three_phase_core_DC_extended = 8
    AC_core3p_DC_extended = 9


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


class EVSENotificationType(Enum):
    None_ = 0
    StopCharging = 1
    ReNegotiation = 2


class faultCodeType(Enum):
    ParsingError = 0
    NoTLSRootCertificatAvailable = 1
    UnknownError = 2


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
    FAILED_EVSEPresentVoltageToLow = 20
    FAILED_MeteringSignatureNotValid = 21
    FAILED_WrongEnergyTransferType = 22


class EVRequestedEnergyTransferType(Enum):
    AC_single_phase_core = 0
    AC_three_phase_core = 1
    DC_core = 2
    DC_extended = 3
    DC_combo_core = 4
    DC_unique = 5


class serviceCategoryType(Enum):
    EVCharging = 0
    Internet = 1
    ContractCertificate = 2
    OtherCustom = 3


class EVSEProcessingType(Enum):
    Finished = 0
    Ongoing = 1


class valueType(Enum):
    bool = 0
    byte = 1
    short = 2
    int = 3
    physicalValue = 4
    string = 5


def CostType(costKind:costKindType, amount:int, amountMultiplier:int|None=None)->dict[str, Any]:
    """
    :param costKind: costKindType
    :param amount: uint32/unsignedInt
    :param amountMultiplier: int8/byte (optional)
    """

    j:dict[str,Any] = {
        "costKind": costKind.value,
        "amount": amount
    }

    if amountMultiplier is not None:
        j["amountMultiplier"] = amountMultiplier

    return j

def RelativeTimeIntervalType(start:int, duration:int|None=None)->dict[str, Any]:
    """
    :param start: uint32/unsignedInt
    :param duration: uint32/unsignedInt (optional)
    """
    
    j:dict[str,Any] = {
        "start": start
    }

    if duration is not None:
        j["duration"] = duration

    return j

def IntervalType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """
    
    j:dict[str,Any] = {
        "unused": _unused
    }
    return j

def ConsumptionCostType(startValue:int, Cost:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param startValue: uint32/unsignedInt
    :param Cost: dict[CostType] (optional)
    """

    j:dict[str, Any] = {
        "startValue": startValue
    }

    if Cost is not None:
        j["Cost"] = Cost

    return j

def TransformType(Algorithm:str, ANY:bytearray|None=None, XPath:str|None=None)->dict[str, Any]:
    """
    :param Algorithm: string
    :param ANY: bytearray (optional)
    :param XPath: string (optional)
    """

    Algorithm_chars = [ord(x) for x in Algorithm]

    j:dict[str,Any] = {
        "Algorithm": {
            "characters": Algorithm_chars,
            "charactersLen": len(Algorithm_chars)
        }
    }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytes": ANY_bytes,
            "bytesLen": len(ANY_bytes)
        }

    if XPath is not None:
        XPath_chars = [ord(x) for x in XPath]
        j["XPath"] = {
            "characters": XPath_chars,
            "charactersLen": len(XPath_chars)
        }

    return j

def PMaxScheduleEntryType(PMax:int, RelativeTimeInterval:dict[str, Any]|None=None, TimeInterval:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param PMax: int16/short
    :param RelativeTimeInterval: dict[RelativeTimeIntervalType] (optional)
    :param TimeInterval: dict[TimeIntervalType] (optional)
    """

    j:dict[str, Any] = {
        "PMax": PMax
    }

    if RelativeTimeInterval is not None:
        j["RelativeTimeInterval"] = RelativeTimeInterval

    if TimeInterval is not None:
        j["TimeInterval"] = TimeInterval

    return j

def SalesTariffEntryType(EPriceLevel:int, RelativeTimeInterval:dict[str, Any]|None=None, TimeInterval:dict[str, Any]|None=None, ConsumptionCost:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param EPriceLevel: uint8/byte
    :param RelativeTimeInterval: dict[RelativeTimeIntervalType] (optional)
    :param TimeInterval: dict[TimeIntervalType] (optional)
    :param ConsumptionCost: dict[ConsumptionCostType] (optional)
    """

    j:dict[str, Any] = {
        "EPriceLevel": EPriceLevel
    }

    if RelativeTimeInterval is not None:
        j["RelativeTimeInterval"] = RelativeTimeInterval

    if TimeInterval is not None:
        j["TimeInterval"] = TimeInterval

    if ConsumptionCost is not None:
        j["ConsumptionCost"] = ConsumptionCost

    return j

def TransformsType(Transform:dict[str, Any])->dict[str, Any]:
    """
    :param Transform: dict[TransformType]
    """

    j:dict[str,Any] = {
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
    j:dict[str,Any] = {
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

def DigestMethodType(Algorithm:str, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param Algorithm: string
    :param ANY: bytearray (optional)
    """

    Algorithm_chars = [ord(x) for x in Algorithm]

    j:dict[str,Any] = {
        "Algorithm": {
            "characters": Algorithm_chars,
            "charactersLen": len(Algorithm_chars)
        }
    }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytes": ANY_bytes,
            "bytesLen": len(ANY_bytes)
        }

    return j

def RSAKeyValyeType(Modulus:bytearray, Exponent:bytearray)->dict[str, Any]:
    """
    :param Modulus: bytearray
    :param Exponent: bytearray
    """

    Modulus_bytes = list(Modulus)
    Exponent_bytes = list(Exponent)

    j:dict[str,Any] = {
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
    
def PMaxScheduleType(PMaxScheduleID:int, PMaxScheduleEntry:list[dict])->dict[str, Any]:
    """
    :param PMaxScheduleID: int16/short
    :param PMaxScheduleEntry: list[dict[PMaxScheduleEntryType]]
    """

    j:dict[str,Any] = {
        "PMaxScheduleID": PMaxScheduleID,
        "PMaxScheduleEntry": PMaxScheduleEntry
    }

    return j

def SalesTariffType(Id:str, SalesTariffID:int, NumEPriceLevels:int, SalesTariffEntry:list[dict], SalesTariffDescription:str|None=None)->dict[str, Any]:
    """
    :param Id: string
    :param SalesTariffID: int16/short
    :param NumEPriceLevels: uint8/unsignedByte
    :param SalesTariffEntry: list[dict[SalesTariffEntryType]]
    :param SalesTariffDescription: string (optional)
    """

    Id_chars = [ord(x) for x in Id]

    j:dict[str, Any] = {
        "Id": {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        },
        "SalesTariffID": SalesTariffID,
        "NumEPriceLevels": NumEPriceLevels,
        "SalesTariffEntry": SalesTariffEntry
    }

    if SalesTariffDescription is not None:
        SalesTariffDescription_chars = [ord(x) for x in SalesTariffDescription]
        j["SalesTariffDescription"] = {
            "characters": SalesTariffDescription_chars,
            "charactersLen": len(SalesTariffDescription_chars)
        }

    return j

def CanonicalizationMethodType(Algorithm:str, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param Algorithm: string
    :param ANY: bytearray (optional)
    """

    Algorithm_chars = [ord(x) for x in Algorithm]

    j:dict[str,Any] = {
        "Algorithm": {
            "characters": Algorithm_chars,
            "charactersLen": len(Algorithm_chars)
        }
    }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytes": ANY_bytes,
            "bytesLen": len(ANY_bytes)
        }

    return j

def ServiceTagType(ServiceID:int, ServiceCategory:serviceCategoryType, ServiceName:str|None=None, ServiceScope:str|None=None)->dict[str, Any]:
    """
    :param ServiceID: uint16/unsignedShort
    :param ServiceCategory: serviceCategoryType
    :param ServiceName: string (optional)
    :param ServiceScope: string (optional)
    """

    j:dict[str, Any] = {
        "ServiceID": ServiceID,
        "ServiceCategory": ServiceCategory.value
    }

    if ServiceName is not None:
        ServiceName_chars = [ord(x) for x in ServiceName]
        j["ServiceName"] = {
            "characters": ServiceName_chars,
            "charactersLen": len(ServiceName_chars)
        }

    if ServiceScope is not None:
        ServiceScope_chars = [ord(x) for x in ServiceScope]
        j["ServiceScope"] = {
            "characters": ServiceScope_chars,
            "charactersLen": len(ServiceScope_chars)
        }

    return j

def ServiceType(ServiceTag:dict[str, Any], FreeService:int)->dict[str, Any]:
    """
    :param ServiceTag: dict[ServiceTagType]
    :param FreeService: boolean
    """

    j:dict[str,Any] = {
        "ServiceTag": ServiceTag,
        "FreeService": FreeService
    }

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

def SAScheduleTupleType(SAScheduleTupleID:int, PMaxSchedule:dict[str, Any]|None=None, SalesTariff:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param SAScheduleTupleID: int16/short
    :param PMaxSchedule: dict[PMaxScheduleType] (optional)
    :param SalesTariff: dict[SalesTariffType] (optional)
    """

    j:dict[str, Any] = {
        "SAScheduleTupleID": SAScheduleTupleID
    }

    if PMaxSchedule is not None:
        j["PMaxSchedule"] = PMaxSchedule

    if SalesTariff is not None:
        j["SalesTariff"] = SalesTariff

    return j

def AC_EVSEStatusType(PowerSwitchClosed:int, RCD:int, NotificationMaxDelay:int, EVSENotification:EVSENotificationType)->dict[str, Any]:
    """
    :param PowerSwitchClosed: boolean
    :param RCD: boolean
    :param NotificationMaxDelay: uint32/unsignedInt
    :param EVSENotification: EVSENotificationType
    """

    j:dict[str,Any] = {
        "PowerSwitchClosed": PowerSwitchClosed,
        "RCD": RCD,
        "NotificationMaxDelay": NotificationMaxDelay,
        "EVSENotification": EVSENotification.value
    }

    return j

def SignatureMethodType(Algorithm:str, HMACOutputLength:dict[str, Any]|None=None, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param Algorithm: string
    :param HMACOutputLength: dict[exi_signed_t] (optional)
    :param ANY: bytearray (optional)
    """

    Algorithm_chars = [ord(x) for x in Algorithm]

    j:dict[str,Any] = {
        "Algorithm": {
            "characters": Algorithm_chars,
            "charactersLen": len(Algorithm_chars)
        }
    }

    if HMACOutputLength is not None:
        j["HMACOutputLength"] = HMACOutputLength

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytes": ANY_bytes,
            "bytesLen": len(ANY_bytes)
        }

    return j

def KeyValueType(DSAKeyValue:dict[str, Any]|None=None, RSAKeyValue:dict[str, Any]|None=None, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param DSAKeyValue: dict[DSAKeyValueType] (optional)
    :param RSAKeyValue: dict[RSAKeyValyeType] (optional)
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
            "bytes": ANY_bytes,
            "bytesLen": len(ANY_bytes)
        }

    return j

def SubCertificatesType(Certificate:bytearray)->dict[str, Any]:
    """
    :param Certificate: bytearray
    """

    Certificate_bytes = list(Certificate)

    j:dict[str,Any] = {
        "Certificate": {
            "bytes": Certificate_bytes,
            "bytesLen": len(Certificate_bytes)
        }
    }

    return j

def ProfileEntryType(ChargingProfileEntryStart:int, ChargingProfileEntryMaxPower:int)->dict[str, Any]:
    """
    :param ChargingProfileEntryStart: uint32/unsignedInt
    :param ChargingProfileEntryMaxPower: int16/short
    """

    j:dict[str,Any] = {
        "ChargingProfileEntryStart": ChargingProfileEntryStart,
        "ChargingProfileEntryMaxPower": ChargingProfileEntryMaxPower
    }

    return j

def ReferenceType(DigestMethod:dict[str, Any], DigestValue:bytearray, Id:str|None=None, Type:str|None=None, URI:str|None=None, Transforms:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param DigestMethod: dict[DigestMethodType]
    :param DigestValue: bytearray
    :param Id: string (optional)
    :param Type: string (optional)
    :param URI: string (optional)
    :param Transforms: dict[TransformsType] (optional)
    """

    DigestValue_bytes = list(DigestValue)

    j:dict[str, Any] = {
        "DigestMethod": DigestMethod,
        "DigestValue": {
            "bytes": DigestValue_bytes,
            "bytesLen": len(DigestValue_bytes)
        }
    }

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    if Type is not None:
        Type_chars = [ord(x) for x in Type]
        j["Type"] = {
            "characters": Type_chars,
            "charactersLen": len(Type_chars)
        }

    if URI is not None:
        URI_chars = [ord(x) for x in URI]
        j["URI"] = {
            "characters": URI_chars,
            "charactersLen": len(URI_chars)
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
        Type_chars = [ord(x) for x in Type]
        j["Type"] = {
            "characters": Type_chars,
            "charactersLen": len(Type_chars)
        }

    if URI is not None:
        URI_chars = [ord(x) for x in URI]
        j["URI"] = {
            "characters": URI_chars,
            "charactersLen": len(URI_chars)
        }

    if Transforms is not None:
        j["Transforms"] = Transforms

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
            "bytes": X509SKI_bytes,
            "bytesLen": len(X509SKI_bytes)
        }

    if X509SubjectName is not None:
        X509SubjectName_chars = [ord(x) for x in X509SubjectName]
        j["X509SubjectName"] = {
            "characters": X509SubjectName_chars,
            "charactersLen": len(X509SubjectName_chars)
        }

    if X509Certificate is not None:
        X509Certificate_bytes = list(X509Certificate)
        j["X509Certificate"] = {
            "bytes": X509Certificate_bytes,
            "bytesLen": len(X509Certificate_bytes)
        }

    if X509CRL is not None:
        X509CRL_bytes = list(X509CRL)
        j["X509CRL"] = {
            "bytes": X509CRL_bytes,
            "bytesLen": len(X509CRL_bytes)
        }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytes": ANY_bytes,
            "bytesLen": len(ANY_bytes)
        }

    return j

# TODO: do something better with union types?
def PGPDataType(choice_1_PGPKeyID:bytearray|None=None, choice_1_PGPKeyPacket:bytearray|None=None, choice_1_ANY:bytearray|None=None, choice_2_PGPKeyPacket:bytearray|None=None, choice_2_ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param choice_1_PGPKeyID: bytearray (optional)
    :param choice_1_PGPKeyPacket: bytearray (optional)
    :param choice_1_ANY: bytearray (optional)
    :param choice_2_PGPKeyPacket: bytearray (optional)
    :param choice_2_ANY: bytearray (optional)
    """

    j:dict[str,Any] = {}

    if choice_1_PGPKeyID is not None:
        choice_1_PGPKeyID_bytes = list(choice_1_PGPKeyID)
        j["choice_1_PGPKeyID"] = {
            "bytes": choice_1_PGPKeyID_bytes,
            "bytesLen": len(choice_1_PGPKeyID_bytes)
        }

    if choice_1_PGPKeyPacket is not None:
        choice_1_PGPKeyPacket_bytes = list(choice_1_PGPKeyPacket)
        j["choice_1_PGPKeyPacket"] = {
            "bytes": choice_1_PGPKeyPacket_bytes,
            "bytesLen": len(choice_1_PGPKeyPacket_bytes)
        }

    if choice_1_ANY is not None:
        choice_1_ANY_bytes = list(choice_1_ANY)
        j["choice_1_ANY"] = {
            "bytes": choice_1_ANY_bytes,
            "bytesLen": len(choice_1_ANY_bytes)
        }

    if choice_2_PGPKeyPacket is not None:
        choice_2_PGPKeyPacket_bytes = list(choice_2_PGPKeyPacket)
        j["choice_2_PGPKeyPacket"] = {
            "bytes": choice_2_PGPKeyPacket_bytes,
            "bytesLen": len(choice_2_PGPKeyPacket_bytes)
        }

    if choice_2_ANY is not None:
        choice_2_ANY_bytes = list(choice_2_ANY)
        j["choice_2_ANY"] = {
            "bytes": choice_2_ANY_bytes,
            "bytesLen": len(choice_2_ANY_bytes)
        }

    return j

def SPKIDataType(SPKIexp:bytearray, ANY:bytearray|None=None)->dict[str, Any]:
    """
    :param SPKIexp: bytearray
    :param ANY: bytearray (optional)
    """

    SPKIexp_bytes = list(SPKIexp)

    j:dict[str,Any] = {
        "SPKIexp": {
            "bytes": SPKIexp_bytes,
            "bytesLen": len(SPKIexp_bytes)
        }
    }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytes": ANY_bytes,
            "bytesLen": len(ANY_bytes)
        }

    return j

def SignedInfoType(CanonicalizationMethod:dict[str, Any], SignatureMethod:dict[str, Any], Reference:dict[str, Any], Id:str|None=None)->dict[str, Any]:
    """
    :param CanonicalizationMethod: dict[CanonicalizationMethodType]
    :param SignatureMethod: dict[SignatureMethodType]
    :param Reference: dict[ReferenceType]
    :param Id: string (optional)
    """

    j:dict[str,Any] = {
        "CanonicalizationMethod": CanonicalizationMethod,
        "SignatureMethod": SignatureMethod,
        "Reference": Reference
    }

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    return j

def DC_EVStatusType(EVReady:int, EVErrorCode:DC_EVErrorCodeType, EVRESSSOC:int, EVCabinConditioning:int|None=None, EVRESSConditioning:int|None=None)->dict[str, Any]:
    """
    :param EVReady: boolean
    :param EVErrorCode: DC_EVErrorCodeType
    :param EVRESSSOC: int8/byte
    :param EVCabinConditioning: boolean (optional)
    :param EVRESSConditioning: boolean (optional)
    """

    j:dict[str,Any] = {
        "EVReady": EVReady,
        "EVErrorCode": EVErrorCode.value,
        "EVRESSSOC": EVRESSSOC
    }

    if EVCabinConditioning is not None:
        j["EVCabinConditioning"] = EVCabinConditioning

    if EVRESSConditioning is not None:
        j["EVRESSConditioning"] = EVRESSConditioning

    return j

def SignatureValueType(CONTENT:bytearray, Id:str|None=None)->dict[str, Any]:
    """
    :param CONTENT: bytearray
    :param Id: string (optional)
    """

    CONTENT_bytes = list(CONTENT)

    j:dict[str,Any] = {
        "CONTENT": {
            "bytes": CONTENT_bytes,
            "bytesLen": len(CONTENT_bytes)
        }
    }

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    return j

def CertificateChainType(Certificate:bytearray, SubCertificates:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param Certificate: bytearray
    :param SubCertificates: dict[SubCertificatesType] (optional)
    """

    Certificate_bytes = list(Certificate)

    j:dict[str,Any] = {
        "Certificate": {
            "bytes": Certificate_bytes,
            "bytesLen": len(Certificate_bytes)
        }
    }

    if SubCertificates is not None:
        j["SubCertificates"] = SubCertificates

    return j

def DC_EVSEStatusType(EVSEStatusCode:DC_EVSEStatusCodeType, NotificationMaxDelay:int, EVSENotification:EVSENotificationType, DC_EVSEIsolationLevel:isolationLevelType|None=None)->dict[str, Any]:
    """
    :param EVSEStatusCode: DC_EVSEStatusCodeType
    :param NotificationMaxDelay: uint32/unsignedInt
    :param EVSENotification: EVSENotificationType
    :param DC_EVSEIsolationLevel: isolationLevelType (optional)
    """

    j:dict[str,Any] = {
        "EVSEStatusCode": EVSEStatusCode.value,
        "NotificationMaxDelay": NotificationMaxDelay,
        "EVSENotification": EVSENotification.value
    }

    if DC_EVSEIsolationLevel is not None:
        j["DC_EVSEIsolationLevel"] = DC_EVSEIsolationLevel.value

    return j

def PhysicalValueType(Multiplier:int, Value:int, Unit:unitSymbolType|None=None)->dict[str, Any]:
    """
    :param Multiplier: int8/byte
    :param Value: int16/short
    :param Unit: unitSymbolType (optional)
    """

    j:dict[str, Any] = {
        "Multiplier": Multiplier,
        "Value": Value
    }

    if Unit is not None:
        j["Unit"] = Unit.value

    return j

def ParameterType(Name:str, ValueType:valueType, boolValue:int|None=None, byteValue:int|None=None, shortValue:int|None=None, intValue:int|None=None, physicalValue:dict[str, Any]|None=None, stringValue:str|None=None)->dict[str, Any]:
    """
    :param Name: string
    :param ValueType: valueType
    :param boolValue: boolean (optional)
    :param byteValue: int8/byte (optional)
    :param shortValue: int16/short (optional)
    :param intValue: int32/int (optional)
    :param physicalValue: dict[PhysicalValueType] (optional)
    :param stringValue: string (optional)
    """

    Name_chars = [ord(x) for x in Name]

    j:dict[str,Any] = {
        "Name": {
            "characters": Name_chars,
            "charactersLen": len(Name_chars)
        },
        "ValueType": ValueType.value
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
        stringValue_chars = [ord(x) for x in stringValue]
        j["stringValue"] = {
            "characters": stringValue_chars,
            "charactersLen": len(stringValue_chars)
        }

    return j

def ParameterSetType(ParameterSetID:int, Parameter:dict[str, Any])->dict[str, Any]:
    """
    :param ParameterSetID: int16/short
    :param Parameter: dict[ParameterType]
    """

    j:dict[str,Any] = {
        "ParameterSetID": ParameterSetID,
        "Parameter": Parameter
    }

    return j

def ListOfRootCertificateIDsType(RootCertificateID:list[str])->dict[str, Any]:
    """
    :param RootCertificateID: list[string]
    """

    j:dict[str,Any] = {
        "RootCertificateID": {
            "arrayLen": len(RootCertificateID),
            "array": [{
                "characters": [ord(x) for x in rcid],
                "charactersLen": len(rcid)
            } for rcid in RootCertificateID]
        }
    }

    return j

def PaymentOptionsType(PaymentOption:list[paymentOptionType])->dict[str, Any]:
    """
    :param PaymentOption: list[paymentOptionType]
    """

    j:dict[str,Any] = {
        "PaymentOption": {
            "arrayLen": len(PaymentOption),
            "array": [po.value for po in PaymentOption]
        }
    }

    return j

def SelectedServiceListType(SelectedService:list[dict])->dict[str, Any]:
    """
    :param SelectedService: list[dict[SelectedServiceType]]
    """

    j:dict[str,Any] = {
        "SelectedService": {
            "arrayLen": len(SelectedService),
            "array": SelectedService
        }
    }

    return j

def AC_EVChargeParameterType(DepartureTime:int, EAmount:dict[str, Any], EMaxVoltage:dict[str, Any], EMaxCurrent:dict[str, Any], EMinCurrent:dict[str, Any])->dict[str, Any]:
    """
    :param DepartureTime: uint32/unsignedInt
    :param EAmount: dict[PhysicalValueType]
    :param EMaxVoltage: dict[PhysicalValueType]
    :param EMaxCurrent: dict[PhysicalValueType]
    :param EMinCurrent: dict[PhysicalValueType]
    """

    j:dict[str,Any] = {
        "DepartureTime": DepartureTime,
        "EAmount": EAmount,
        "EMaxVoltage": EMaxVoltage,
        "EMaxCurrent": EMaxCurrent,
        "EMinCurrent": EMinCurrent
    }

    return j

def DC_EVChargeParameterType(DC_EVStatus:dict[str, Any], EVMaximumCurrentLimit:dict[str, Any], EVMaximumVoltageLimit:dict[str, Any], EVMaximumPowerLimit:dict[str, Any]|None=None, EVEnergyCapacity:dict[str, Any]|None=None, EVEnergyRequest:dict[str, Any]|None=None, FullSOC:int|None=None, BulkSOC:int|None=None)->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    :param EVMaximumCurrentLimit: dict[PhysicalValueType]
    :param EVMaximumVoltageLimit: dict[PhysicalValueType]
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

def EV_ChargeParameterType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str,Any] = {
        "unused": _unused
    }
    return j

def ChargingProfileType(SAScheduleTupleID:int, ProfileEntry:list[dict])->dict[str, Any]:
    """
    :param SAScheduleTupleID: int16/short
    :param ProfileEntry: list[dict[ProfileEntryType]]
    """

    j:dict[str,Any] = {
        "SAScheduleTupleID": SAScheduleTupleID,
        "ProfileEntry": ProfileEntry
    }

    return j

def EVSEStatusType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str,Any] = {
        "unused": _unused
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

    j:dict[str,Any] = {}

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    if KeyName is not None:
        KeyName_chars = [ord(x) for x in KeyName]
        j["KeyName"] = {
            "characters": KeyName_chars,
            "charactersLen": len(KeyName_chars)
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
        MgmtData_chars = [ord(x) for x in MgmtData]
        j["MgmtData"] = {
            "characters": MgmtData_chars,
            "charactersLen": len(MgmtData_chars)
        }

    if ANY is not None:
        ANY_bytes = list(ANY)
        j["ANY"] = {
            "bytes": ANY_bytes,
            "bytesLen": len(ANY_bytes)
        }

    return j

def ServiceChargeType(ServiceTag:dict[str, Any], FreeService:int, EnergyTransferType:EVSESupportedEnergyTransferType)->dict[str, Any]:
    """
    :param ServiceTag: dict[ServiceTagType]
    :param FreeService: boolean
    :param EnergyTransferType: EVSESupportedEnergyTransferType
    """

    j:dict[str,Any] = {
        "ServiceTag": ServiceTag,
        "FreeService": FreeService,
        "EnergyTransferType": EnergyTransferType.value
    }

    return j

def SAScheduleListType(SAScheduleTuple:list[dict])->dict[str, Any]:
    """
    :param SAScheduleTuple: list[dict[SAScheduleTupleType]]
    """

    j:dict[str,Any] = {
        "SAScheduleTuple": {
            "arrayLen": len(SAScheduleTuple),
            "array": SAScheduleTuple
        }
    }

    return j

def SASchedulesType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str,Any] = {
        "unused": _unused
    }
    return j

def DC_EVPowerDeliveryParameterType(DC_EVStatus:dict[str, Any], ChargingComplete:int, BulkChargingComplete:int|None=None)->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    :param ChargingComplete: boolean
    :param BulkChargingComplete: boolean (optional)
    """

    j:dict[str,Any] = {
        "DC_EVStatus": DC_EVStatus,
        "ChargingComplete": ChargingComplete
    }

    if BulkChargingComplete is not None:
        j["BulkChargingComplete"] = BulkChargingComplete

    return j

def EVPowerDeliveryParameterType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str,Any] = {
        "unused": _unused
    }
    return j

def ObjectType(Encoding:str|None=None, Id:str|None=None, MimeType:str|None=None, Object:bytearray|None=None)->dict[str, Any]:
    """
    :param Encoding: string (optional)
    :param Id: string (optional)
    :param MimeType: string (optional)
    :param Object: bytearray (optional)
    """

    j:dict[str,Any] = {}

    if Encoding is not None:
        Encoding_chars = [ord(x) for x in Encoding]
        j["Encoding"] = {
            "characters": Encoding_chars,
            "charactersLen": len(Encoding_chars)
        }

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    if MimeType is not None:
        MimeType_chars = [ord(x) for x in MimeType]
        j["MimeType"] = {
            "characters": MimeType_chars,
            "charactersLen": len(MimeType_chars)
        }

    if Object is not None:
        Object_bytes = list(Object)
        j["Object"] = {
            "bytes": Object_bytes,
            "bytesLen": len(Object_bytes)
        }

    return j

def ServiceTagListType(Service:dict[str, Any])->dict[str, Any]:
    """
    :param Service: dict[ServiceType]
    """

    j:dict[str,Any] = {
        "Service": Service
    }

    return j

def DC_EVSEChargeParameterType(DC_EVSEStatus:dict[str, Any], EVSEMaximumCurrentLimit:dict[str, Any], EVSEMaximumVoltageLimit:dict[str, Any], EVSEMinimumCurrentLimit:dict[str, Any], EVSEMinimumVoltageLimit:dict[str, Any], EVSEPeakCurrentRipple:dict[str, Any], EVSEMaximumPowerLimit:dict[str, Any]|None=None, EVSECurrentRegulationTolerance:dict[str, Any]|None=None, EVSEEnergyToBeDelivered:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEMaximumCurrentLimit: dict[PhysicalValueType]
    :param EVSEMaximumVoltageLimit: dict[PhysicalValueType]
    :param EVSEMinimumCurrentLimit: dict[PhysicalValueType]
    :param EVSEMinimumVoltageLimit: dict[PhysicalValueType]
    :param EVSEPeakCurrentRipple: dict[PhysicalValueType]
    :param EVSEMaximumPowerLimit: dict[PhysicalValueType] (optional)
    :param EVSECurrentRegulationTolerance: dict[PhysicalValueType] (optional)
    :param EVSEEnergyToBeDelivered: dict[PhysicalValueType] (optional)
    """

    j:dict[str,Any] = {
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEMaximumCurrentLimit": EVSEMaximumCurrentLimit,
        "EVSEMaximumVoltageLimit": EVSEMaximumVoltageLimit,
        "EVSEMinimumCurrentLimit": EVSEMinimumCurrentLimit,
        "EVSEMinimumVoltageLimit": EVSEMinimumVoltageLimit,
        "EVSEPeakCurrentRipple": EVSEPeakCurrentRipple
    }

    if EVSEMaximumPowerLimit is not None:
        j["EVSEMaximumPowerLimit"] = EVSEMaximumPowerLimit

    if EVSECurrentRegulationTolerance is not None:
        j["EVSECurrentRegulationTolerance"] = EVSECurrentRegulationTolerance

    if EVSEEnergyToBeDelivered is not None:
        j["EVSEEnergyToBeDelivered"] = EVSEEnergyToBeDelivered

    return j

def AC_EVSEChargeParameterType(AC_EVSEStatus:dict[str, Any], EVSEMaxVoltage:dict[str, Any], EVSEMaxCurrent:dict[str, Any], EVSEMinCurrent:dict[str, Any])->dict[str, Any]:
    """
    :param AC_EVSEStatus: dict[AC_EVSEStatusType]
    :param EVSEMaxVoltage: dict[PhysicalValueType]
    :param EVSEMaxCurrent: dict[PhysicalValueType]
    :param EVSEMinCurrent: dict[PhysicalValueType]
    """

    j:dict[str,Any] = {
        "AC_EVSEStatus": AC_EVSEStatus,
        "EVSEMaxVoltage": EVSEMaxVoltage,
        "EVSEMaxCurrent": EVSEMaxCurrent,
        "EVSEMinCurrent": EVSEMinCurrent
    }

    return j

def EVSEChargeParameterType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str,Any] = {
        "unused": _unused
    }
    return j

def MeterInfoType(MeterID:str, MeterReading:dict[str, Any]|None=None, SigMeterReading:bytearray|None=None, MeterStatus:int|None=None, TMeter:int|None=None)->dict[str, Any]:
    """
    :param MeterID: string
    :param MeterReading: dict[MeterReadingType] (optional)
    :param SigMeterReading: bytearray (optional)
    :param MeterStatus: int16/short (optional)
    :param TMeter: int64/long (optional)
    """

    MeterID_chars = [ord(x) for x in MeterID]

    j:dict[str, Any] = {
        "MeterID": {
            "characters": MeterID_chars,
            "charactersLen": len(MeterID_chars)
        }
    }

    if MeterReading is not None:
        j["MeterReading"] = MeterReading

    if SigMeterReading is not None:
        SigMeterReading_bytes = list(SigMeterReading)
        j["SigMeterReading"] = {
            "bytes": SigMeterReading_bytes,
            "bytesLen": len(SigMeterReading_bytes)
        }

    if MeterStatus is not None:
        j["MeterStatus"] = MeterStatus

    if TMeter is not None:
        j["TMeter"] = TMeter

    return j

def CertificateInstallationResType(Id:str, ResponseCode:responseCodeType, ContractSignatureCertChain:dict[str, Any], ContractSignatureEncryptedPrivateKey:bytearray, DHParams:bytearray, ContractID:str)->dict[str, Any]:
    """
    :param Id: string
    :param ResponseCode: responseCodeType
    :param ContractSignatureCertChain: dict[CertificateChainType]
    :param ContractSignatureEncryptedPrivateKey: bytearray
    :param DHParams: bytearray
    :param ContractID: string
    """

    Id_chars = [ord(x) for x in Id]
    ContractID_chars = [ord(x) for x in ContractID]
    ContractSignatureEncryptedPrivateKey_bytes = list(ContractSignatureEncryptedPrivateKey)
    DHParams_bytes = list(DHParams)

    j:dict[str,Any] = {
        "Id": {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        },
        "ResponseCode": ResponseCode.value,
        "ContractSignatureCertChain": ContractSignatureCertChain,
        "ContractSignatureEncryptedPrivateKey": {
            "bytes": ContractSignatureEncryptedPrivateKey_bytes,
            "bytesLen": len(ContractSignatureEncryptedPrivateKey_bytes)
        },
        "DHParams": {
            "bytes": DHParams_bytes,
            "bytesLen": len(DHParams_bytes)
        },
        "ContractID": {
            "characters": ContractID_chars,
            "charactersLen": len(ContractID_chars)
        }
    }

    return j

def CableCheckReqType(DC_EVStatus:dict[str, Any])->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    """

    j:dict[str,Any] = {
        "DC_EVStatus": DC_EVStatus
    }

    return j

def CableCheckResType(ResponseCode:responseCodeType, DC_EVSEStatus:dict[str, Any], EVSEProcessing:EVSEProcessingType)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEProcessing: EVSEProcessingType
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value,
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEProcessing": EVSEProcessing.value
    }

    return j

def PreChargeReqType(DC_EVStatus:dict[str, Any], EVTargetVoltage:dict[str, Any], EVTargetCurrent:dict[str, Any])->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    :param EVTargetVoltage: dict[PhysicalValueType]
    :param EVTargetCurrent: dict[PhysicalValueType]
    """

    j:dict[str,Any] = {
        "DC_EVStatus": DC_EVStatus,
        "EVTargetVoltage": EVTargetVoltage,
        "EVTargetCurrent": EVTargetCurrent
    }

    return j

def PreChargeResType(ResponseCode:responseCodeType, DC_EVSEStatus:dict[str, Any], EVSEPresentVoltage:dict[str, Any])->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEPresentVoltage: dict[PhysicalValueType]
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value,
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEPresentVoltage": EVSEPresentVoltage
    }

    return j

def CurrentDemandReqType(DC_EVStatus:dict[str, Any], EVTargetCurrent:dict[str, Any], ChargingComplete:int, EVTargetVoltage:dict[str, Any], EVMaximumVoltageLimit:dict[str, Any]|None=None, EVMaximumCurrentLimit:dict[str, Any]|None=None, EVMaximumPowerLimit:dict[str, Any]|None=None, BulkChargingComplete:int|None=None, RemainingTimeToFullSOC:dict[str, Any]|None=None, RemainingTimeToBulkSOD:dict[str, Any]|None=None)->dict[str, Any]:
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
    :param RemainingTimeToBulkSOD: dict[PhysicalValueType] (optional)
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

    if RemainingTimeToBulkSOD is not None:
        j["RemainingTimeToBulkSOD"] = RemainingTimeToBulkSOD

    return j

def CurrentDemandResType(ResponseCode:responseCodeType, DC_EVSEStatus:dict[str, Any], EVSEPresentVoltage:dict[str, Any], EVSEPresentCurrent:dict[str, Any], EVSECurrentLimitAchieved:int, EVSEVoltageLimitAchieved:int, EVSEPowerLimitAchieved:int, EVSEMaximumVoltageLimit:dict[str, Any]|None=None, EVSEMaximumCurrentLimit:dict[str, Any]|None=None, EVSEMaximumPowerLimit:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEPresentVoltage: dict[PhysicalValueType]
    :param EVSEPresentCurrent: dict[PhysicalValueType]
    :param EVSECurrentLimitAchieved: boolean
    :param EVSEVoltageLimitAchieved: boolean
    :param EVSEPowerLimitAchieved: boolean
    :param EVSEMaximumVoltageLimit: dict[PhysicalValueType] (optional)
    :param EVSEMaximumCurrentLimit: dict[PhysicalValueType] (optional)
    :param EVSEMaximumPowerLimit: dict[PhysicalValueType] (optional)
    """

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEPresentVoltage": EVSEPresentVoltage,
        "EVSEPresentCurrent": EVSEPresentCurrent,
        "EVSECurrentLimitAchieved": EVSECurrentLimitAchieved,
        "EVSEVoltageLimitAchieved": EVSEVoltageLimitAchieved,
        "EVSEPowerLimitAchieved": EVSEPowerLimitAchieved
    }

    if EVSEMaximumVoltageLimit is not None:
        j["EVSEMaximumVoltageLimit"] = EVSEMaximumVoltageLimit

    if EVSEMaximumCurrentLimit is not None:
        j["EVSEMaximumCurrentLimit"] = EVSEMaximumCurrentLimit

    if EVSEMaximumPowerLimit is not None:
        j["EVSEMaximumPowerLimit"] = EVSEMaximumPowerLimit

    return j

def WeldingDetectionReqType(DC_EVStatus:dict[str, Any])->dict[str, Any]:
    """
    :param DC_EVStatus: dict[DC_EVStatusType]
    """

    j:dict[str,Any] = {
        "DC_EVStatus": DC_EVStatus
    }

    return j

def WeldingDetectionResType(ResponseCode:responseCodeType, DC_EVSEStatus:dict[str, Any], EVSEPresentVoltage:dict[str, Any])->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param DC_EVSEStatus: dict[DC_EVSEStatusType]
    :param EVSEPresentVoltage: dict[PhysicalValueType]
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value,
        "DC_EVSEStatus": DC_EVSEStatus,
        "EVSEPresentVoltage": EVSEPresentVoltage
    }

    return j

def SessionSetupReqType(EVCCID:bytearray)->dict[str, Any]:
    """
    :param EVCCID: bytearray
    """

    EVCCID_bytes = list(EVCCID)

    j:dict[str,Any] = {
        "EVCCID": {
            "bytes": EVCCID_bytes,
            "bytesLen": len(EVCCID_bytes)
        }
    }

    return j

def CertificateInstallationReqType(OEMProvisioningCert:bytearray, ListOfRootCertificateIDs:dict[str, Any], DHParams:bytearray, Id:str|None=None)->dict[str, Any]:
    """
    :param OEMProvisioningCert: bytearray
    :param ListOfRootCertificateIDs: dict[ListOfRootCertificateIDsType]
    :param DHParams: bytearray
    :param Id: string (optional)
    """

    OEMProvisioningCert_bytes = list(OEMProvisioningCert)
    DHParams_bytes = list(DHParams)

    j:dict[str,Any] = {
        "OEMProvisioningCert": {
            "bytes": OEMProvisioningCert_bytes,
            "bytesLen": len(OEMProvisioningCert_bytes)
        },
        "ListOfRootCertificateIDs": ListOfRootCertificateIDs,
        "DHParams": {
            "bytes": DHParams_bytes,
            "bytesLen": len(DHParams_bytes)
        }
    }

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    return j

def SessionSetupResType(ResponseCode:responseCodeType, EVSEID:str, DateTimeNow:int|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param EVSEID: string
    :param DateTimeNow: int64/long (optional)
    """

    EVSEID_chars = [ord(x) for x in EVSEID]

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "EVSEID": {
            "characters": EVSEID_chars,
            "charactersLen": len(EVSEID_chars)
        }
    }

    if DateTimeNow is not None:
        j["DateTimeNow"] = DateTimeNow

    return j

def ServiceDiscoveryReqType(ServiceScope:str|None=None, ServiceCategory:serviceCategoryType|None=None)->dict[str, Any]:
    """
    :param ServiceScope: string (optional)
    :param ServiceCategory: serviceCategoryType (optional)
    """

    j:dict[str,Any] = {}

    if ServiceScope is not None:
        ServiceScope_chars = [ord(x) for x in ServiceScope]
        j["ServiceScope"] = {
            "characters": ServiceScope_chars,
            "charactersLen": len(ServiceScope_chars)
        }

    if ServiceCategory is not None:
        j["ServiceCategory"] = ServiceCategory.value

    return j

def ServiceDiscoveryResType(ResponseCode:responseCodeType, PaymentOptions:dict[str, Any], ChargeService:dict[str, Any], ServiceList:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param PaymentOptions: dict[PaymentOptionsType]
    :param ChargeService: dict[ServiceChargeType]
    :param ServiceList: dict[ServiceTagListType] (optional)
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value,
        "PaymentOptions": PaymentOptions,
        "ChargeService": ChargeService
    }

    if ServiceList is not None:
        j["ServiceList"] = ServiceList

    return j

def ServiceDetailReqType(ServiceID:int)->dict[str, Any]:
    """
    :param ServiceID: uint16/short
    """

    j:dict[str,Any] = {
        "ServiceID": ServiceID
    }

    return j

def ServiceDetailResType(ResponseCode:responseCodeType, ServiceID:int, ServiceParameterList:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param ServiceID: uint16/short
    :param ServiceParameterList: dict[ServiceParameterListType] (optional)
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value,
        "ServiceID": ServiceID
    }

    if ServiceParameterList is not None:
        j["ServiceParameterList"] = ServiceParameterList

    return j

def ServicePaymentSelectionReqType(SelectedPaymentOption:paymentOptionType, SelectedServiceList:dict[str, Any])->dict[str, Any]:
    """
    :param SelectedPaymentOption: paymentOptionType
    :param SelectedServiceList: dict[SelectedServiceListType]
    """

    j:dict[str,Any] = {
        "SelectedPaymentOption": SelectedPaymentOption.value,
        "SelectedServiceList": SelectedServiceList
    }

    return j

def ServicePaymentSelectionResType(ResponseCode:responseCodeType)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value
    }

    return j

def PaymentDetailsReqType(ContractID:str, ContractSignatureCertChain:dict[str, Any])->dict[str, Any]:
    """
    :param ContractID: string
    :param ContractSignatureCertChain: dict[CertificateChainType]
    """

    ContractID_chars = [ord(x) for x in ContractID]

    j:dict[str,Any] = {
        "ContractID": {
            "characters": ContractID_chars,
            "charactersLen": len(ContractID_chars)
        },
        "ContractSignatureCertChain": ContractSignatureCertChain
    }

    return j

def PaymentDetailsResType(ResponseCode:responseCodeType, GenChallenge:str, DateTimeNow:int)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param GenChallenge: string
    :param DateTimeNow: int64/long
    """

    GenChallenge_chars = [ord(x) for x in GenChallenge]

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value,
        "GenChallenge": {
            "characters": GenChallenge_chars,
            "charactersLen": len(GenChallenge_chars)
        },
        "DateTimeNow": DateTimeNow
    }

    return j

def ContractAuthenticationReqType(Id:str|None=None, GenChallenge:str|None=None)->dict[str, Any]:
    """
    :param Id: string (optional)
    :param GenChallenge: string (optional)
    """

    j:dict[str,Any] = {}

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    if GenChallenge is not None:
        GenChallenge_chars = [ord(x) for x in GenChallenge]
        j["GenChallenge"] = {
            "characters": GenChallenge_chars,
            "charactersLen": len(GenChallenge_chars)
        }

    return j

def ContractAuthenticationResType(ResponseCode:responseCodeType, EVSEProcessing:EVSEProcessingType)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param EVSEProcessing: EVSEProcessingType
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value,
        "EVSEProcessing": EVSEProcessing.value
    }

    return j

def ChargeParameterDiscoveryReqType(EVRequestedEnergyTransferType:EVRequestedEnergyTransferType, AC_EVChargeParameter:dict[str, Any]|None=None, DC_EVChargeParameter:dict[str, Any]|None=None, EV_ChargeParameter:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param EVRequestedEnergyTransferType: EVRequestedEnergyTransferType
    :param AC_EVChargeParameter: dict[AC_EVChargeParameterType] (optional)
    :param DC_EVChargeParameter: dict[DC_EVChargeParameterType] (optional)
    :param EV_ChargeParameter: dict[EV_ChargeParameterType] (optional)
    """

    j:dict[str,Any] = {
        "EVRequestedEnergyTransferType": EVRequestedEnergyTransferType.value
    }

    if AC_EVChargeParameter is not None:
        j["AC_EVChargeParameter"] = AC_EVChargeParameter

    if DC_EVChargeParameter is not None:
        j["DC_EVChargeParameter"] = DC_EVChargeParameter

    if EV_ChargeParameter is not None:
        j["EV_ChargeParameter"] = EV_ChargeParameter

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

    j:dict[str,Any] = {
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

def PowerDeliveryReqType(ReadyToChargeState:int, ChargingProfile:dict[str, Any]|None=None, DC_EVPowerDeliveryParameter:dict[str, Any]|None=None, EVPowerDeliveryParameter:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ReadyToChargeState: boolean
    :param ChargingProfile: dict[ChargingProfileType] (optional)
    :param DC_EVPowerDeliveryParameter: dict[DC_EVPowerDeliveryParameterType] (optional)
    :param EVPowerDeliveryParameter: dict[EVPowerDeliveryParameterType] (optional)
    """

    j:dict[str,Any] = {
        "ReadyToChargeState": ReadyToChargeState
    }

    if ChargingProfile is not None:
        j["ChargingProfile"] = ChargingProfile

    if DC_EVPowerDeliveryParameter is not None:
        j["DC_EVPowerDeliveryParameter"] = DC_EVPowerDeliveryParameter

    if EVPowerDeliveryParameter is not None:
        j["EVPowerDeliveryParameter"] = EVPowerDeliveryParameter

    return j

def PowerDeliveryResType(ResponseCode:responseCodeType, AC_EVSEStatus:dict[str, Any]|None=None, DC_EVSEStatus:dict[str, Any]|None=None, EVSEStatus:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param AC_EVSEStatus: dict[AC_EVSEStatusType] (optional)
    :param DC_EVSEStatus: dict[DC_EVSEStatusType] (optional)
    :param EVSEStatus: dict[EVSEStatusType] (optional)
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value
    }

    if AC_EVSEStatus is not None:
        j["AC_EVSEStatus"] = AC_EVSEStatus

    if DC_EVSEStatus is not None:
        j["DC_EVSEStatus"] = DC_EVSEStatus

    if EVSEStatus is not None:
        j["EVSEStatus"] = EVSEStatus

    return j

def ChargingStatusReqType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str,Any] = {
        "unused": _unused
    }
    return j

def ChargingStatusResType(ResponseCode:responseCodeType, EVSEID:bytearray, SAScheduleTupleID:int, ReceiptRequired:int, AC_EVSEStatus:dict[str, Any], EVSEMaxCurrent:dict[str, Any]|None=None, MeterInfo:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param EVSEID: bytearray
    :param SAScheduleTupleID: int16/short
    :param ReceiptRequired: boolean
    :param AC_EVSEStatus: dict[AC_EVSEStatusType]
    :param EVSEMaxCurrent: dict[PhysicalValueType] (optional)
    :param MeterInfo: dict[MeterInfoType] (optional)
    """

    EVSEID_bytes = list(EVSEID)

    j:dict[str, Any] = {
        "ResponseCode": ResponseCode.value,
        "EVSEID": {
            "bytes": EVSEID_bytes,
            "bytesLen": len(EVSEID_bytes)
        },
        "SAScheduleTupleID": SAScheduleTupleID,
        "ReceiptRequired": ReceiptRequired,
        "AC_EVSEStatus": AC_EVSEStatus
    }

    if EVSEMaxCurrent is not None:
        j["EVSEMaxCurrent"] = EVSEMaxCurrent

    if MeterInfo is not None:
        j["MeterInfo"] = MeterInfo

    return j

def MeteringReceiptReqType(SessionID:bytearray, MeterInfo:dict[str, Any], Id:str|None=None, SAScheduleTupleID:int|None=None)->dict[str, Any]:
    """
    :param SessionID: bytearray
    :param MeterInfo: dict[MeterInfoType]
    :param Id: string (optional)
    :param SAScheduleTupleID: int16/short (optional)
    """

    SessionID_bytes = list(SessionID)

    j:dict[str,Any] = {
        "SessionID": {
            "bytes": SessionID_bytes,
            "bytesLen": len(SessionID_bytes)
        },
        "MeterInfo": MeterInfo
    }

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    if SAScheduleTupleID is not None:
        j["SAScheduleTupleID"] = SAScheduleTupleID

    return j

def MeteringReceiptResType(ResponseCode:responseCodeType, AC_EVSEStatus:dict[str, Any])->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    :param AC_EVSEStatus: dict[AC_EVSEStatusType]
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value,
        "AC_EVSEStatus": AC_EVSEStatus
    }

    return j

def SessionStopReqType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str,Any] = {
        "unused": _unused
    }
    return j

def SessionStopResType(ResponseCode:responseCodeType)->dict[str, Any]:
    """
    :param ResponseCode: responseCodeType
    """

    j:dict[str,Any] = {
        "ResponseCode": ResponseCode.value
    }

    return j

def CertificateUpdateReqType(ContractID:str, ListOfRootCertificateIDs:dict[str, Any], DHParams:bytearray, Id:str|None=None)->dict[str, Any]:
    """
    :param ContractID: string
    :param ListOfRootCertificateIDs: dict[ListOfRootCertificateIDsType]
    :param DHParams: bytearray
    :param Id: string (optional)
    """

    ContractID_chars = [ord(x) for x in ContractID]
    DHParams_bytes = list(DHParams)

    j:dict[str,Any] = {
        "ContractID": {
            "characters": ContractID_chars,
            "charactersLen": len(ContractID_chars)
        },
        "ListOfRootCertificateIDs": ListOfRootCertificateIDs,
        "DHParams": {
            "bytes": DHParams_bytes,
            "bytesLen": len(DHParams_bytes)
        }
    }

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    return j

def CertificateUpdateResType(Id:str, ResponseCode:responseCodeType, ContractSignatureCertChain:dict[str, Any], ContractSignatureEncryptedPrivateKey:bytearray, DHParams:bytearray, ContractID:str, RetryCounter:int)->dict[str, Any]:
    """
    :param Id: string
    :param ResponseCode: responseCodeType
    :param ContractSignatureCertChain: dict[CertificateChainType]
    :param ContractSignatureEncryptedPrivateKey: bytearray
    :param DHParams: bytearray
    :param ContractID: string
    :param RetryCounter: int16/short
    """

    Id_chars = [ord(x) for x in Id]
    ContractID_chars = [ord(x) for x in ContractID]
    ContractSignatureEncryptedPrivateKey_bytes = list(ContractSignatureEncryptedPrivateKey)
    DHParams_bytes = list(DHParams)

    j:dict[str,Any] = {
        "Id": {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        },
        "ResponseCode": ResponseCode.value,
        "ContractSignatureCertChain": ContractSignatureCertChain,
        "ContractSignatureEncryptedPrivateKey": {
            "bytes": ContractSignatureEncryptedPrivateKey_bytes,
            "bytesLen": len(ContractSignatureEncryptedPrivateKey_bytes)
        },
        "DHParams": {
            "bytes": DHParams_bytes,
            "bytesLen": len(DHParams_bytes)
        },
        "ContractID": {
            "characters": ContractID_chars,
            "charactersLen": len(ContractID_chars)
        },
        "RetryCounter": RetryCounter
    }

    return j

def BodyBaseType(_unused:int)->dict[str, Any]:
    """
    :param _unused: int
    """

    j:dict[str,Any] = {
        "unused": _unused
    }
    return j

def NotificationType(FaultCode:faultCodeType, FaultMsg:str|None=None)->dict[str, Any]:
    """
    :param FaultCode: faultCodeType
    :param FaultMsg: string (optional)
    """

    j:dict[str,Any] = {
        "FaultCode": FaultCode.value
    }

    if FaultMsg is not None:
        FaultMsg_chars = [ord(x) for x in FaultMsg]
        j["FaultMsg"] = {
            "characters": FaultMsg_chars,
            "charactersLen": len(FaultMsg_chars)
        }

    return j

def SignatureType(SignedInfo:dict[str, Any], SignatureValue:bytearray, Id:str|None=None, KeyInfo:dict[str, Any]|None=None, Object:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param SignedInfo: dict[SignedInfoType]
    :param SignatureValue: bytearray
    :param Id: string (optional)
    :param KeyInfo: dict[KeyInfoType] (optional)
    :param Object: dict[ObjectType] (optional)
    """

    SignatureValue_bytes = list(SignatureValue)

    j:dict[str,Any] = {
        "SignedInfo": SignedInfo,
        "SignatureValue": {
            "bytes": SignatureValue_bytes,
            "bytesLen": len(SignatureValue_bytes)
        }
    }

    if Id is not None:
        Id_chars = [ord(x) for x in Id]
        j["Id"] = {
            "characters": Id_chars,
            "charactersLen": len(Id_chars)
        }

    if KeyInfo is not None:
        j["KeyInfo"] = KeyInfo

    if Object is not None:
        j["Object"] = Object

    return j

def MessageHeaderType(SessionID:bytearray, Notification:dict[str, Any]|None=None, Signature:dict[str, Any]|None=None)->dict[str, Any]:
    """
    :param SessionID: bytearray
    :param Notification: dict[NotificationType] (optional)
    :param Signature: dict[SignatureType] (optional)
    """

    SessionID_bytes = list(SessionID)

    j:dict[str,Any] = {
        "SessionID": {
            "bytes": SessionID_bytes,
            "bytesLen": len(SessionID_bytes)
        }
    }

    if Notification is not None:
        j["Notification"] = Notification

    if Signature is not None:
        j["Signature"] = Signature

    return j

def BodyType(BodyElement:dict[str, Any]|None=None, CableCheckReq:dict[str, Any]|None=None, CableCheckRes:dict[str, Any]|None=None, CertificateInstallationReq:dict[str, Any]|None=None, CertificateInstallationRes:dict[str, Any]|None=None, CertificateUpdateReq:dict[str, Any]|None=None, CertificateUpdateRes:dict[str, Any]|None=None, ChargeParameterDiscoveryReq:dict[str, Any]|None=None, ChargeParameterDiscoveryRes:dict[str, Any]|None=None, ChargingStatusReq:dict[str, Any]|None=None, ChargingStatusRes:dict[str, Any]|None=None, ContractAuthenticationReq:dict[str, Any]|None=None, ContractAuthenticationRes:dict[str, Any]|None=None, CurrentDemandReq:dict[str, Any]|None=None, CurrentDemandRes:dict[str, Any]|None=None, MeteringReceiptReq:dict[str, Any]|None=None, MeteringReceiptRes:dict[str, Any]|None=None, PaymentDetailsReq:dict[str, Any]|None=None, PaymentDetailsRes:dict[str, Any]|None=None, PowerDeliveryReq:dict[str, Any]|None=None, PowerDeliveryRes:dict[str, Any]|None=None, PreChargeReq:dict[str, Any]|None=None, PreChargeRes:dict[str, Any]|None=None, ServiceDetailReq:dict[str, Any]|None=None, ServiceDetailRes:dict[str, Any]|None=None, ServiceDiscoveryReq:dict[str, Any]|None=None, ServiceDiscoveryRes:dict[str, Any]|None=None, ServicePaymentSelectionReq:dict[str, Any]|None=None, ServicePaymentSelectionRes:dict[str, Any]|None=None, SessionSetupReq:dict[str, Any]|None=None, SessionSetupRes:dict[str, Any]|None=None, SessionStopReq:dict[str, Any]|None=None, SessionStopRes:dict[str, Any]|None=None, WeldingDetectionReq:dict[str, Any]|None=None, WeldingDetectionRes:dict[str, Any]|None=None)->dict[str, Any]:
    """
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
    :param ContractAuthenticationReq: dict[ContractAuthenticationReqType] (optional)
    :param ContractAuthenticationRes: dict[ContractAuthenticationResType] (optional)
    :param CurrentDemandReq: dict[CurrentDemandReqType] (optional)
    :param CurrentDemandRes: dict[CurrentDemandResType] (optional)
    :param MeteringReceiptReq: dict[MeteringReceiptReqType] (optional)
    :param MeteringReceiptRes: dict[MeteringReceiptResType] (optional)
    :param PaymentDetailsReq: dict[PaymentDetailsReqType] (optional)
    :param PaymentDetailsRes: dict[PaymentDetailsResType] (optional)
    :param PowerDeliveryReq: dict[PowerDeliveryReqType] (optional)
    :param PowerDeliveryRes: dict[PowerDeliveryResType] (optional)
    :param PreChargeReq: dict[PreChargeReqType] (optional)
    :param PreChargeRes: dict[PreChargeResType] (optional)
    :param ServiceDetailReq: dict[ServiceDetailReqType] (optional)
    :param ServiceDetailRes: dict[ServiceDetailResType] (optional)
    :param ServiceDiscoveryReq: dict[ServiceDiscoveryReqType] (optional)
    :param ServiceDiscoveryRes: dict[ServiceDiscoveryResType] (optional)
    :param ServicePaymentSelectionReq: dict[ServicePaymentSelectionReqType] (optional)
    :param ServicePaymentSelectionRes: dict[ServicePaymentSelectionResType] (optional)
    :param SessionSetupReq: dict[SessionSetupReqType] (optional)
    :param SessionSetupRes: dict[SessionSetupResType] (optional)
    :param SessionStopReq: dict[SessionStopType] (optional)
    :param SessionStopRes: dict[SessionStopResType] (optional)
    :param WeldingDetectionReq: dict[WeldingDetectionReqType] (optional)
    :param WeldingDetectionRes: dict[WeldingDetectionResType] (optional)
    """

    j:dict[str,Any] = {}

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

    if ContractAuthenticationReq is not None:
        j["ContractAuthenticationReq"] = ContractAuthenticationReq

    if ContractAuthenticationRes is not None:
        j["ContractAuthenticationRes"] = ContractAuthenticationRes

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

    if ServicePaymentSelectionReq is not None:
        j["ServicePaymentSelectionReq"] = ServicePaymentSelectionReq

    if ServicePaymentSelectionRes is not None:
        j["ServicePaymentSelectionRes"] = ServicePaymentSelectionRes

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

    j:dict[str,Any] = {
        "Header": Header,
        "Body": Body
    }

    return j

def exiDocument(V2G_Message:dict[str, Any])->dict[str, Any]:
    """
    :param V2G_Message: dict[V2G_MessageType]
    """

    j:dict[str,Any] = {
        "V2G_Message": V2G_Message
    }

    return j