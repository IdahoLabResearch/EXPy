"""XSD-choice manifest for the ISO 15118-20 ACDP Namespace.

The ACDP schema only defines an `exiDocument` root (no Fragment /
xmldsigFragment), but the document's union reaches the full W3C XML-DSig
type tree directly. ACDP's own message bodies are flat sequences — they
contain no `xs:choice` particles of their own — so the only entries here
are the xmldsig tree shared with the other ISO-20 namespaces.

See `iso2_choices.py` for the manifest format.
"""

CHOICES: dict[str, list[list[list[str]]]] = {
    # xmldsig types — same choice tree as the other ISO-20 namespaces
    # (XML signatures are W3C standard, libcbv2g reuses the same schema).
    "iso20_acdp_TransformType": [
        [["ANY"], ["XPath"]],
    ],
    "iso20_acdp_KeyValueType": [
        [["DSAKeyValue"], ["RSAKeyValue"], ["ANY"]],
    ],
    "iso20_acdp_X509DataType": [
        [
            ["X509IssuerSerial"],
            ["X509SKI"],
            ["X509SubjectName"],
            ["X509Certificate"],
            ["X509CRL"],
            ["ANY"],
        ],
    ],
    "iso20_acdp_KeyInfoType": [
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
    "iso20_acdp_PGPDataType": [
        [["choice_1"], ["choice_2"]],
    ],
    # SignaturePropertyType: C struct flags ANY as `_isUsed`-optional, but the
    # XSD `xs:choice` underneath requires at least one element.
    "iso20_acdp_SignaturePropertyType": [
        [["ANY"]],
    ],
}
