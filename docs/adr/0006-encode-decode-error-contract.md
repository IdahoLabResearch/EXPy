# Encode/Decode Error Contract on Invalid Input

The six Processor methods (Document, Fragment, XmldsigFragment × encode, decode) wrap libcbv2g's encoder/decoder for one Namespace each. Two failure surfaces exist at the C boundary: libcbv2g's own return code (negative `EXI_ERROR__*` constants on encoder/decoder failure such as bitstream overflow), and the generated marshalers' `nlohmann::json` accesses, which abort the process on missing-key / wrong-type input. #20 hardened the first surface: `*Processor.cpp` now captures the libcbv2g return code, surfaces it through `encoded_data.status` / `decoded_data.status`, and Python raises typed `EncodeError` / `DecodeError`. The second surface — the nlohmann marshaler abort — is unchanged and remains the target of a separate follow-up (#21). The integration suite still isolates roundtrips in a forked child (`tests/integration/conftest.py:17`) so that the remaining abort path doesn't crash the runner.

## Contract

**encode (`encode`, `encode_fragment`, `encode_xmldsig`)**

- Returns `bytes` on success.
- Raises `EncodeError` when libcbv2g returns a non-zero status (e.g., `EXI_ERROR__BITSTREAM_OVERFLOW` when a valid payload exceeds the 256-byte static buffer). Message names the Namespace and root: `"{namespace} {root}: libcbv2g rc={status}"`.
- On structurally invalid payloads that reach the generated marshaler (missing required nested field, wrong-typed scalar), behavior is still **undefined**: `nlohmann::json::operator[]` and typed accessors abort the process before encode runs. #21 will guard this path.
- Raises `NotImplementedError` if the Processor's schema does not define the requested root (`EXIProcessor.py`).
- Propagates `TypeError` / `ValueError` from `json.dumps` when the input is not JSON-serializable. Caller-bug errors at the Python boundary, not contract failures.
- Callers handling untrusted payloads still **must** isolate encode (subprocess, sandbox) until #21 lands; the marshaler-abort path is the residual risk, not the libcbv2g RC path.

**decode (`decode`, `decode_fragment`, `decode_xmldsig`)**

- Returns a `dict` in the EVerest JSON shape on well-formed input.
- Raises `DecodeError` when libcbv2g returns a non-zero status (e.g., empty bitstream, malformed EXI header). Message names the Namespace and root.
- On bytes that libcbv2g accepts as well-formed but that are not actually valid for the schema, the dict content is **undefined** — may be empty, may be partially populated. The "no fixed-point on garbage" property still holds for the negative-input smoke layer.
- Raises `NotImplementedError` if the Processor's schema does not define the requested root.
- Same isolation requirement as encode for the residual marshaler-abort path (which is rarely triggered on decode, but possible).

## Considered Options

- **Keep `bytes | None` on encode.** Rejected once the RC path landed: `None` carries no information about which Namespace / root failed; `EncodeError` carries both plus the libcbv2g rc. Two failure shapes (None vs exception) for one concept is friction with no benefit. AcCCS compat was a non-issue per project owner.
- **Add a Python-side schema validator before encode.** Still rejected: duplicates the schema knowledge that already lives in the generated marshalers, and ADR-0005's schema-driven fixture generator is the authoritative source for what a valid payload looks like. Sharing that machinery with a pre-flight validator is a larger refactor — out of scope here and tracked under #21.
- **Wrap decode with a guard that catches native aborts.** #20 chose the cheaper path: read libcbv2g's return code (the cpp wrappers were discarding it). The remaining nlohmann abort path on decode is rare and is the target of #21.

## Consequences

- The negative-test smoke layer (#17) still pins the *weak* contract (None | raise | abort) because the marshaler-abort path is real for the second failure surface. `test_decode_errors.py` and `test_encode_errors.py` (added by #20) pin the *strong* contract — that the libcbv2g RC path raises a typed exception with namespace + root in the message.
- The public API docs (#8) should be updated alongside this hardening: encode is now `bytes` (not `bytes | None`), raising `EncodeError` on libcbv2g failure; decode raises `DecodeError`. The "may abort on malformed input" warning remains but is narrower — it applies to the marshaler-abort path only.
- One follow-up remains: #21 (decode guard / forked worker or pre-flight marshaler-key validation) so untrusted input can be made safe without subprocess isolation in every consumer.
