# EXPy

Python and C/C++ bindings over LF Energy EVerest's `libcbv2g`, providing per-namespace EXI encode/decode for V2G charging protocols (DIN 70121, ISO 15118-2, ISO 15118-20). This file is a glossary only — no implementation details, no decisions. For decisions see `docs/adr/`.

## Language

**Processor**:
A per-namespace codec instance bound to one `libcbv2g` encoder/decoder pair, loaded as a single `.so` (e.g., `ISO2Processor`, `ISO20DCProcessor`).
_Avoid_: Codec, encoder, decoder (those name the C functions Processor wraps, not the wrapper itself).

**Namespace**:
The dispatch key identifying which schema a message belongs to. EXPy supports eight: `HANDSHAKE`, `DIN`, `ISO2`, `ISO20_COMMON`, `ISO20_AC`, `ISO20_DC`, `ISO20_WPT`, `ISO20_ACDP`. One Namespace ↔ one **Processor**.
_Avoid_: Protocol (DIN, ISO‑2, and ISO‑20 are protocols; "ISO‑20 AC" is not — it's a namespace within ISO‑20).

**Document**:
A full V2G message envelope (e.g., `iso2_exiDocument`), the standard unit produced and consumed in a charging session.
_Avoid_: Message, payload.

**Fragment**:
A sub-element of a **Document** encoded or decoded standalone. Required for PnC signature computation and contract certificate installation. Has its own root type in `libcbv2g` (`*_exiFragment`), distinct from a Document.
_Avoid_: Partial, snippet, sub-message.

**XmldsigFragment**:
A specific **Fragment** variant rooted at `SignedInfo` (W3C XML Digital Signatures). Used only for PnC signature payloads. Distinct from a generic Fragment.
_Avoid_: SignedInfo fragment, signature fragment.

**Marshaler**:
A generated pair of functions (`getJson_<Type>` / `setDoc_<Type>`) that maps between a single `libcbv2g` C struct and its JSON representation. One pair per type per **Processor**.
_Avoid_: Serializer, mapper, converter.

**EVerest JSON shape**:
The JSON conventions EXPy emits and accepts: byte fields as `{"bytes": [...], "bytesLen": N}`, optional sub-elements marked with `"isUsed": true`, root keyed by message type name. Inherited from `libcbv2g` conventions; consumers either match this shape or translate at their boundary.
_Avoid_: JSON format, schema, wire format.

## Relationships

- One **Namespace** maps to exactly one **Processor**.
- A **Processor** exposes encode/decode for **Documents**, **Fragments**, and (where the underlying schema defines it) **XmldsigFragments**.
- A **Processor** is composed of many **Marshalers** — one per C struct type in its `libcbv2g` schema.
- All Processors emit and accept the **EVerest JSON shape**.

## Example dialogue

> **Consumer:** "I need to compute a PnC signature over a `CertificateInstallationReq` — do I call `encode` on it?"
> **EXPy author:** "No — `encode` produces a full **Document**. For the signature you need the **Fragment** API, since the signed payload is a sub-element. And the signature itself goes through the **XmldsigFragment** API because its root is `SignedInfo`."
> **Consumer:** "Same `Processor` for both?"
> **EXPy author:** "Yes — `ISO2Processor` handles all three for the ISO‑2 namespace. Different namespaces need different Processors."

## Flagged ambiguities

- "Protocol" was used loosely to mean both *protocol* (DIN, ISO‑2, ISO‑20) and *namespace* (one of the eight schema groups). Resolved: **Namespace** is the dispatch unit; **Protocol** is the V2G standard a Namespace belongs to. ISO‑20 is one Protocol containing five Namespaces.
