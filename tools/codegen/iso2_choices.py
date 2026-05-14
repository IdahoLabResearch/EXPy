"""XSD-choice manifest for the ISO 15118-2 (ISO2) namespace.

The libcbv2g C headers do not distinguish XSD-choice groups from
sibling-optional fields — both look like `T NAME; unsigned int NAME_isUsed:1;`.
This manifest records the choice groupings so the fixture generator can emit
one roundtrip scenario per branch instead of attempting all-on / all-off
variants the encoder cannot honor.

Keys are libcbv2g C-struct names; values are a list of branches, each branch
a list of field names that get `_isUsed=1` together. Fields outside any
branch are non-choice members of the struct and are emitted as usual.

When a choice branch contains a nested struct that itself has a choice, the
generator defaults to branch 0 of the inner choice. The inner choice gets
its own per-branch coverage at whichever top-level root reaches it directly.
"""

CHOICES: dict[str, list[list[str]]] = {
    # SalesTariffEntryType — xs:choice of {RelativeTimeInterval, TimeInterval}.
    # Reached as an array element of SalesTariffType, which is itself a
    # Fragment root.
    "iso2_SalesTariffEntryType": [
        ["RelativeTimeInterval"],
        ["TimeInterval"],
    ],
}
