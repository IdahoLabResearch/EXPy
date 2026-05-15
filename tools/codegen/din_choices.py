"""XSD-choice manifest for the DIN 70121 namespace.

Same format as `iso2_choices.py` — see that module's docstring for the full
explanation of the `dict[str, list[list[list[str]]]]` shape (groups ->
branches -> fields). DIN's Document tree mirrors ISO2's at the choice
points the generator cares about, with `din_` prefixed C-struct names.
"""

CHOICES: dict[str, list[list[list[str]]]] = {
    # SalesTariffEntryType — {RelativeTimeInterval, TimeInterval}. Reached
    # transitively from ChargeParameterDiscoveryRes via SAScheduleList ->
    # SAScheduleTuple -> SalesTariff.
    "din_SalesTariffEntryType": [
        [["RelativeTimeInterval"], ["TimeInterval"]],
    ],
    # PMaxScheduleEntryType — same {RelativeTimeInterval, TimeInterval}
    # choice. Reached via SAScheduleList -> SAScheduleTuple -> PMaxSchedule.
    "din_PMaxScheduleEntryType": [
        [["RelativeTimeInterval"], ["TimeInterval"]],
    ],
    # ChargeParameterDiscoveryReqType — EVChargeParameter kind choice.
    "din_ChargeParameterDiscoveryReqType": [
        [
            ["AC_EVChargeParameter"],
            ["DC_EVChargeParameter"],
            ["EVChargeParameter"],
        ],
    ],
    # ChargeParameterDiscoveryResType — two independent xs:choice particles:
    # SAScheduleList/SASchedules, and {AC,DC,EVSE}ChargeParameter (same
    # pattern as ISO2; the SA list pair was confirmed empirically there).
    "din_ChargeParameterDiscoveryResType": [
        [["SAScheduleList"], ["SASchedules"]],
        [
            ["AC_EVSEChargeParameter"],
            ["DC_EVSEChargeParameter"],
            ["EVSEChargeParameter"],
        ],
    ],
    # PowerDeliveryReqType — EVPowerDeliveryParameter kind choice. DIN does
    # not have an AC_EVPowerDeliveryParameter variant.
    "din_PowerDeliveryReqType": [
        [
            ["DC_EVPowerDeliveryParameter"],
            ["EVPowerDeliveryParameter"],
        ],
    ],
    # PowerDeliveryResType — EVSEStatus kind choice (AC/DC/abstract).
    "din_PowerDeliveryResType": [
        [
            ["AC_EVSEStatus"],
            ["DC_EVSEStatus"],
            ["EVSEStatus"],
        ],
    ],
    # ParameterType — value-kind choice. Name + ValueType attributes are
    # required non-choice. Reached from ServiceDetailRes via
    # ServiceParameterList -> ParameterSet -> Parameter.
    "din_ParameterType": [
        [
            ["boolValue"],
            ["byteValue"],
            ["shortValue"],
            ["intValue"],
            ["physicalValue"],
            ["stringValue"],
        ],
    ],
}
