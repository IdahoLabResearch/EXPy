"""XSD-choice manifest for the ISO 15118-20 DC Namespace.

The DC schema reuses the W3C XML-DSig choice tree (Signature/KeyInfo/...);
those entries mirror `iso20_common_choices.py`. The DC-specific message
types layer their own xs:choice on top — each of the four message bodies
picks exactly one EnergyTransferMode / ControlMode flavor.

See `iso2_choices.py` for the manifest format.
"""

CHOICES: dict[str, list[list[list[str]]]] = {
    # DC_ChargeParameterDiscoveryReq/Res — xs:choice between BPT and non-BPT
    # energy-transfer-mode variants.
    "iso20_dc_DC_ChargeParameterDiscoveryReqType": [
        [["DC_CPDReqEnergyTransferMode"], ["BPT_DC_CPDReqEnergyTransferMode"]],
    ],
    "iso20_dc_DC_ChargeParameterDiscoveryResType": [
        [["DC_CPDResEnergyTransferMode"], ["BPT_DC_CPDResEnergyTransferMode"]],
    ],
    # DC_ChargeLoopReq/Res — xs:choice across Dynamic / Scheduled / generic
    # ControlMode variants (BPT and non-BPT for the two main flavors).
    "iso20_dc_DC_ChargeLoopReqType": [
        [
            ["BPT_Dynamic_DC_CLReqControlMode"],
            ["BPT_Scheduled_DC_CLReqControlMode"],
            ["CLReqControlMode"],
            ["Dynamic_DC_CLReqControlMode"],
            ["Scheduled_DC_CLReqControlMode"],
        ],
    ],
    "iso20_dc_DC_ChargeLoopResType": [
        [
            ["BPT_Dynamic_DC_CLResControlMode"],
            ["BPT_Scheduled_DC_CLResControlMode"],
            ["CLResControlMode"],
            ["Dynamic_DC_CLResControlMode"],
            ["Scheduled_DC_CLResControlMode"],
        ],
    ],
    # xmldsig types — same choice tree as ISO-2 / ISO-20 Common
    # (XML signatures are W3C standard, libcbv2g reuses the same schema).
    "iso20_dc_TransformType": [
        [["ANY"], ["XPath"]],
    ],
    "iso20_dc_KeyValueType": [
        [["DSAKeyValue"], ["RSAKeyValue"], ["ANY"]],
    ],
    "iso20_dc_X509DataType": [
        [
            ["X509IssuerSerial"],
            ["X509SKI"],
            ["X509SubjectName"],
            ["X509Certificate"],
            ["X509CRL"],
            ["ANY"],
        ],
    ],
    "iso20_dc_KeyInfoType": [
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
    # Xmldsig: PGPDataType — see ADR-0010 for the iso20 grammar rewrite.
    "iso20_dc_PGPDataType": [
        [["choice_1"], ["choice_2"]],
    ],
    # SignaturePropertyType: C struct flags ANY as `_isUsed`-optional, but the
    # XSD `xs:choice` underneath requires at least one element.
    "iso20_dc_SignaturePropertyType": [
        [["ANY"]],
    ],
}
