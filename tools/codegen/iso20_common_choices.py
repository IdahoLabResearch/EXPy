"""XSD-choice manifest for the ISO 15118-20 Common Messages Namespace.

See `iso2_choices.py` for the manifest format. Entries below capture
xs:choice particles whose default min/max emission cannot satisfy the
libcbv2g encoder — the encoder honours only one branch per choice, so
`__maximal` scenarios that set all sibling `_isUsed` bits round-trip as
the truncated single-branch form and fail.
"""

CHOICES: dict[str, list[list[list[str]]]] = {
    # xmldsig types — same choice tree as ISO-2 (XML signatures are W3C
    # standard, libcbv2g reuses the same schema).
    "iso20_TransformType": [
        [["ANY"], ["XPath"]],
    ],
    "iso20_KeyValueType": [
        [["DSAKeyValue"], ["RSAKeyValue"], ["ANY"]],
    ],
    "iso20_X509DataType": [
        [
            ["X509IssuerSerial"],
            ["X509SKI"],
            ["X509SubjectName"],
            ["X509Certificate"],
            ["X509CRL"],
            ["ANY"],
        ],
    ],
    "iso20_KeyInfoType": [
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
    # ScheduleExchangeReq/Res — xs:choice of control-mode flavour
    # (Dynamic vs Scheduled). Both sides have the same shape.
    "iso20_ScheduleExchangeReqType": [
        [["Dynamic_SEReqControlMode"], ["Scheduled_SEReqControlMode"]],
    ],
    "iso20_ScheduleExchangeResType": [
        [["Dynamic_SEResControlMode"], ["Scheduled_SEResControlMode"]],
    ],
    # SignedMeteringData — xs:choice of {Dynamic_SMDTControlMode,
    # Scheduled_SMDTControlMode}. Receipt is an independent optional sibling.
    "iso20_SignedMeteringDataType": [
        [["Dynamic_SMDTControlMode"], ["Scheduled_SMDTControlMode"]],
    ],
    # SignedInstallationData — xs:choice between encrypted-private-key
    # variants by ECDH curve.
    "iso20_SignedInstallationDataType": [
        [
            ["SECP521_EncryptedPrivateKey"],
            ["X448_EncryptedPrivateKey"],
            ["TPM_EncryptedPrivateKey"],
        ],
    ],
    # AuthorizationSetupRes / AuthorizationReq — choice of authorization mode
    # (External Identification Means vs Plug-and-Charge).
    "iso20_AuthorizationSetupResType": [
        [["EIM_ASResAuthorizationMode"], ["PnC_ASResAuthorizationMode"]],
    ],
    "iso20_AuthorizationReqType": [
        [["EIM_AReqAuthorizationMode"], ["PnC_AReqAuthorizationMode"]],
    ],
    # SignaturePropertyType — the C struct flags ANY as `_isUsed`-optional,
    # but the XSD `xs:choice` underneath requires at least one element. With
    # only Target+attributes set, the encoder refuses to emit a property body.
    # Single-branch entry forces ANY on.
    "iso20_SignaturePropertyType": [
        [["ANY"]],
    ],
}
