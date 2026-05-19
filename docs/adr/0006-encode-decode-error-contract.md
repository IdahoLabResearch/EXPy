# Encode/Decode Error Contract on Invalid Input

The six Processor methods (Document, Fragment, XmldsigFragment × encode, decode) have no documented behavior on invalid input today. `encode` is declared to return `bytes | None` (`EXIProcessor.py:88–100`), and does return `None` on the narrow path where the C side allocates no output buffer (`EXIProcessor.py:79–80`); however, the generated marshalers access JSON keys directly via `nlohmann::json::operator[]`, which **aborts the process** when a required key is missing, and throws `nlohmann::json::type_error` (also aborting) when a field has the wrong type. The "abort on invalid payload" path therefore exists on encode as well as decode. `decode` calls libcbv2g directly with no return-code check (`src/ISO2Processor.cpp:33–47`, `src/DINProcessor.cpp:34–48`); malformed EXI can likewise abort the process via the underlying C library. The integration suite already isolates every roundtrip in a forked child for this reason (`tests/integration/conftest.py:17`). This ADR codifies the contract as it stands so that consumers, the negative-test smoke layer (#17), and future hardening work share a single reference. Implementation hardening is explicitly out of scope and is filed as separate follow-ups.

## Contract

**encode (`encode`, `encode_fragment`, `encode_xmldsig`)**

- Returns `bytes` on success.
- Returns `None` on the narrow C-side path where the encoder produces no output buffer.
- On structurally invalid payloads (missing required nested field, wrong-typed scalar, length-constrained field violated), behavior is **undefined**: the generated marshalers access keys via `nlohmann::json::operator[]` (asserts on miss) and cast via typed accessors (throws on type mismatch); both paths abort the process.
- Raises `NotImplementedError` if the underlying Processor's libcbv2g schema does not define the requested root (`EXIProcessor.py:74`).
- Propagates `TypeError` / `ValueError` from `json.dumps` when the input is not JSON-serializable (e.g., a `set`, a custom class). These are caller-bug errors at the Python boundary, not contract failures.
- Callers handling untrusted payloads **must** isolate encode (subprocess, sandbox) until the hardening follow-up lands. The `assert_roundtrip` helper in `conftest.py` is the reference pattern for both encode and decode.

**decode (`decode`, `decode_fragment`, `decode_xmldsig`)**

- Returns a `dict` in the EVerest JSON shape on well-formed input.
- On malformed input the dict content is **undefined** — may be empty, may be partially populated, may raise `json.JSONDecodeError` if the C side emits invalid UTF-8, or may abort via a native libcbv2g abort.
- **No fixed-point on garbage.** For arbitrary bytes `b` that are not a well-formed EXI bitstream, `encode(decode(b)) != b`. Garbage in is not silently round-tripped as if it were valid input. This is the one positive guarantee on malformed decode and is what the smoke layer pins down.
- Raises `NotImplementedError` if the Processor's schema does not define the requested root.
- Same isolation requirement as encode.

## Considered Options

- **Switch encode to raising a typed `EncodeError` instead of returning `None`.** Rejected for this push: a breaking API change for all current consumers (AcCCS depends on the `bytes | None` shape), and the C path still cannot say *which* field failed — the exception would carry no more information than `None` does. Revisit when libcbv2g surfaces marshaler diagnostics.
- **Add a Python-side schema validator before encode.** Rejected for this push: duplicates the schema knowledge that already lives in the generated marshalers, and the schema-driven fixture generator (ADR-0005) is the authoritative source for what a valid payload looks like. Sharing that machinery with a pre-flight validator is a larger refactor.
- **Wrap decode with a guard that catches native aborts and raises `DecodeError`.** Rejected for this push, kept as the target end-state: requires either checking libcbv2g return codes (the generated decode wrappers discard them today) or running decode in a forked worker inside the library. Both are real implementation work and belong in a follow-up.

## Consequences

- The negative-test smoke layer (#17) treats both encode and decode as "may reject by returning `None`, raising, or aborting the child"; tests are subprocess-isolated and pass when the child does not produce a valid encoded buffer / decoded dict. This is a weak contract on purpose — it pins down the rejection envelope without committing to the rejection *mechanism*, which the hardening follow-ups will tighten.
- The public API docs (#8) document `bytes | None` for encode and call out that both encode and decode "may abort on malformed input — isolate untrusted calls in a subprocess until hardening lands."
- Two follow-up issues are filed: #20 wires libcbv2g encode/decode return codes through `*Processor.cpp` and raises typed `EncodeError`/`DecodeError`; #21 adds a decode guard (forked worker or pre-flight magic-byte check) so untrusted decode can be made safe without subprocess isolation in every consumer.
- Until those follow-ups land, the contract is "document the floor, not the ceiling": callers know the minimum guarantees, and any future tightening (e.g., decode raising instead of aborting) is a strict refinement, not a breaking change.
