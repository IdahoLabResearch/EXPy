"""XSD-choice manifest for the ISO 15118-2 (ISO2) namespace.

The libcbv2g C headers do not distinguish XSD-choice groups from
sibling-optional fields — both look like `T NAME; unsigned int NAME_isUsed:1;`.
This manifest records the choice groupings so the fixture generator can emit
one roundtrip scenario per branch instead of attempting all-on / all-off
variants the encoder cannot honor.

Format: keys are libcbv2g C-struct names; values are a list of *groups*.
Each group is a list of *branches*, and each branch is a list of field names
that get `_isUsed=1` together. A struct that contains two independent
xs:choice particles (e.g. ChargeParameterDiscoveryRes) appears as a list of
two groups; the generator emits the Cartesian product of branch selections
across groups. Fields outside every group are non-choice members and are
emitted as usual.

When a choice branch contains a nested struct that itself has a choice, the
generator defaults to branch 0 of the inner choice. The inner choice gets
its own per-branch coverage at whichever top-level root reaches it directly.
"""

CHOICES: dict[str, list[list[list[str]]]] = {
    # SalesTariffEntryType — xs:choice of {RelativeTimeInterval, TimeInterval}.
    # Reached as an array element of SalesTariffType, which is itself a
    # Fragment root, and transitively from ChargeParameterDiscoveryRes via
    # SAScheduleList -> SAScheduleTuple -> SalesTariff.
    "iso2_SalesTariffEntryType": [
        [["RelativeTimeInterval"], ["TimeInterval"]],
    ],
    # PMaxScheduleEntryType — same {RelativeTimeInterval, TimeInterval} choice
    # as SalesTariffEntryType. Reached from ChargeParameterDiscoveryRes via
    # SAScheduleList -> SAScheduleTuple -> PMaxSchedule -> PMaxScheduleEntry[].
    "iso2_PMaxScheduleEntryType": [
        [["RelativeTimeInterval"], ["TimeInterval"]],
    ],
    # TransformType — xs:choice of {ANY, XPath}. Algorithm attribute is
    # required and outside the choice. Reached transitively from SignedInfo,
    # Reference, RetrievalMethod, Signature (all Xmldsig + Fragment roots).
    "iso2_TransformType": [
        [["ANY"], ["XPath"]],
    ],
    # ChargeParameterDiscoveryReqType — xs:choice of EVChargeParameter kind.
    # MaxEntriesSAScheduleTuple (optional) and RequestedEnergyTransferMode
    # (required) are non-choice members.
    "iso2_ChargeParameterDiscoveryReqType": [
        [
            ["AC_EVChargeParameter"],
            ["DC_EVChargeParameter"],
            ["EVChargeParameter"],
        ],
    ],
    # ChargeParameterDiscoveryResType — two independent xs:choice particles:
    # SAScheduleList/SASchedules, and {AC,DC,EVSE}ChargeParameter. Empirical:
    # setting both SAScheduleList+SASchedules causes the encoder to emit only
    # one and the roundtrip drops the other.
    "iso2_ChargeParameterDiscoveryResType": [
        [["SAScheduleList"], ["SASchedules"]],
        [
            ["AC_EVSEChargeParameter"],
            ["DC_EVSEChargeParameter"],
            ["EVSEChargeParameter"],
        ],
    ],
    # PowerDeliveryReqType — xs:choice of EVPowerDeliveryParameter kind.
    # ChargeProgress, SAScheduleTupleID (required) and ChargingProfile
    # (optional) are non-choice members.
    "iso2_PowerDeliveryReqType": [
        [
            ["DC_EVPowerDeliveryParameter"],
            ["EVPowerDeliveryParameter"],
        ],
    ],
    # PowerDeliveryResType — xs:choice of EVSEStatus kind.
    "iso2_PowerDeliveryResType": [
        [
            ["AC_EVSEStatus"],
            ["DC_EVSEStatus"],
            ["EVSEStatus"],
        ],
    ],
    # MeteringReceiptResType — xs:choice of EVSEStatus kind.
    "iso2_MeteringReceiptResType": [
        [
            ["AC_EVSEStatus"],
            ["DC_EVSEStatus"],
            ["EVSEStatus"],
        ],
    ],
    # ParameterType — xs:choice across value kinds. Name (attribute) is
    # required non-choice. Reached from ServiceDetailRes via
    # ServiceParameterList -> ParameterSet -> Parameter.
    "iso2_ParameterType": [
        [
            ["boolValue"],
            ["byteValue"],
            ["shortValue"],
            ["intValue"],
            ["physicalValue"],
            ["stringValue"],
        ],
    ],
    # Xmldsig: KeyInfoType — xs:choice across 8 key descriptor kinds. Id
    # attribute is optional non-choice.
    "iso2_KeyInfoType": [
        [
            ["KeyName"],
            ["KeyValue"],
            ["RetrievalMethod"],
            ["X509Data"],
            ["PGPData"],
            ["SPKIData"],
            ["MgmtData"],
            ["ANY"],
        ],
    ],
    # Xmldsig: KeyValueType — xs:choice of {DSAKeyValue, RSAKeyValue, ANY}.
    "iso2_KeyValueType": [
        [["DSAKeyValue"], ["RSAKeyValue"], ["ANY"]],
    ],
    # Xmldsig: X509DataType — xs:choice across X.509 data kinds. Empirical
    # quirk noted in the xmldsig xfail: the encoder emits X509IssuerSerial
    # by default even when nothing is marked _isUsed; the per-branch path
    # avoids that by setting exactly one flag.
    "iso2_X509DataType": [
        [
            ["X509IssuerSerial"],
            ["X509SKI"],
            ["X509SubjectName"],
            ["X509Certificate"],
            ["X509CRL"],
            ["ANY"],
        ],
    ],
    # NB: PGPDataType is intentionally omitted. The libcbv2g header places
    # the `choice_N_isUsed` flags *inside* the same union as the choice_N
    # substructs (lines ~804-840 of iso_2/iso2_msgDefDatatypes.h), so the
    # encoder cannot distinguish the active branch and the decoder returns
    # an empty PGPData. Filed as a separate libcbv2g limitation rather than
    # an XSD-choice generator problem.
}
