"""XSD-choice manifest for the ISO 15118-20 AC Namespace.

The AC schema reuses the W3C XML-DSig choice tree (Signature/KeyInfo/...);
those entries mirror `iso20_common_choices.py`. The AC-specific message
types layer their own xs:choice on top — each of the four message bodies
picks exactly one EnergyTransferMode / ControlMode flavor.

See `iso2_choices.py` for the manifest format.
"""

CHOICES: dict[str, list[list[list[str]]]] = {
    # AC_ChargeParameterDiscoveryReq/Res — xs:choice between BPT and non-BPT
    # energy-transfer-mode variants.
    "iso20_ac_AC_ChargeParameterDiscoveryReqType": [
        [["AC_CPDReqEnergyTransferMode"], ["BPT_AC_CPDReqEnergyTransferMode"]],
    ],
    "iso20_ac_AC_ChargeParameterDiscoveryResType": [
        [["AC_CPDResEnergyTransferMode"], ["BPT_AC_CPDResEnergyTransferMode"]],
    ],
    # AC_ChargeLoopReq/Res — xs:choice across Dynamic / Scheduled / generic
    # ControlMode variants (BPT and non-BPT for the two main flavors).
    "iso20_ac_AC_ChargeLoopReqType": [
        [
            ["BPT_Dynamic_AC_CLReqControlMode"],
            ["BPT_Scheduled_AC_CLReqControlMode"],
            ["CLReqControlMode"],
            ["Dynamic_AC_CLReqControlMode"],
            ["Scheduled_AC_CLReqControlMode"],
        ],
    ],
    "iso20_ac_AC_ChargeLoopResType": [
        [
            ["BPT_Dynamic_AC_CLResControlMode"],
            ["BPT_Scheduled_AC_CLResControlMode"],
            ["CLResControlMode"],
            ["Dynamic_AC_CLResControlMode"],
            ["Scheduled_AC_CLResControlMode"],
        ],
    ],
    # xmldsig types — same choice tree as ISO-2 / ISO-20 Common
    # (XML signatures are W3C standard, libcbv2g reuses the same schema).
    "iso20_ac_TransformType": [
        [["ANY"], ["XPath"]],
    ],
    "iso20_ac_KeyValueType": [
        [["DSAKeyValue"], ["RSAKeyValue"], ["ANY"]],
    ],
    "iso20_ac_X509DataType": [
        [
            ["X509IssuerSerial"],
            ["X509SKI"],
            ["X509SubjectName"],
            ["X509Certificate"],
            ["X509CRL"],
            ["ANY"],
        ],
    ],
    "iso20_ac_KeyInfoType": [
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
    # SignaturePropertyType: C struct flags ANY as `_isUsed`-optional, but the
    # XSD `xs:choice` underneath requires at least one element.
    "iso20_ac_SignaturePropertyType": [
        [["ANY"]],
    ],
}
