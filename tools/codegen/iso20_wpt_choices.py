"""XSD-choice manifest for the ISO 15118-20 WPT Namespace.

The WPT schema only defines an `exiDocument` root (no Fragment /
xmldsigFragment), but the document's union reaches the full W3C XML-DSig
type tree directly. WPT's own message bodies are flat sequences — they
contain no `xs:choice` particles of their own — so the only entries here
are the xmldsig tree shared with the other ISO-20 namespaces.

See `iso2_choices.py` for the manifest format.
"""

CHOICES: dict[str, list[list[list[str]]]] = {
    # LF_SystemSetupData (FinePositioningSetupReq/Res) — xs:choice between
    # the EV-side and primary-device-side setup payload. The libcbv2g
    # encoder grammar (ID=151) emits exactly one branch.
    "iso20_wpt_WPT_LF_SystemSetupDataType": [
        [["LF_TransmitterSetupData"], ["LF_ReceiverSetupData"]],
    ],
    # WPT_LF_DataPackage (FinePositioningReq/Res, via LF_DataPackageList) —
    # xs:choice between the Tx-side and Rx-side data lists. Encoder grammar
    # ID=92 emits exactly one branch.
    "iso20_wpt_WPT_LF_DataPackageType": [
        [["LF_TxData"], ["LF_RxData"]],
    ],
    # xmldsig types — same choice tree as ISO-2 / ISO-20 Common
    # (XML signatures are W3C standard, libcbv2g reuses the same schema).
    "iso20_wpt_TransformType": [
        [["ANY"], ["XPath"]],
    ],
    "iso20_wpt_KeyValueType": [
        [["DSAKeyValue"], ["RSAKeyValue"], ["ANY"]],
    ],
    "iso20_wpt_X509DataType": [
        [
            ["X509IssuerSerial"],
            ["X509SKI"],
            ["X509SubjectName"],
            ["X509Certificate"],
            ["X509CRL"],
            ["ANY"],
        ],
    ],
    "iso20_wpt_KeyInfoType": [
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
    "iso20_wpt_SignaturePropertyType": [
        [["ANY"]],
    ],
}
