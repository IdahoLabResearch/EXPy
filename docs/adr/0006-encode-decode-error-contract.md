# Encode/Decode Error Contract on Invalid Input

The six Processor methods (Document, Fragment, XmldsigFragment × encode, decode) wrap libcbv2g's encoder/decoder for one Namespace each. Two failure surfaces exist at the C boundary: libcbv2g's own return code (negative `EXI_ERROR__*` constants on encoder/decoder failure such as bitstream overflow), and the generated marshalers' `nlohmann::json` accesses (`json::parse`, const `operator[]` on missing keys, `template get<T>()` on wrong-typed values), which historically aborted the process. #20 hardened the first surface by capturing the libcbv2g return code through `encoded_data.status` / `decoded_data.status` so Python raises typed `EncodeError` / `DecodeError`. #21 closes the second surface: each `extern "C"` entry point in `*Processor.cpp` now wraps its body in `try/catch`, and `include/common.hpp` overrides nlohmann's `JSON_ASSERT` to throw instead of aborting. Caught exceptions surface as `EXPY_ERROR__MARSHALER_INPUT` (rc = -1000, distinct from libcbv2g's -1 .. -299 range), which Python translates into the same typed exceptions. The roundtrip subprocess in `tests/integration/conftest.py:_roundtrip_worker` is preserved as belt-and-suspenders against future regressions, but it is no longer the only thing standing between consumers and a native abort.

## Contract

**encode (`encode`, `encode_fragment`, `encode_xmldsig`)**

- Returns `bytes` on success.
- Raises `EncodeError` when libcbv2g returns a non-zero status (e.g., `EXI_ERROR__BITSTREAM_OVERFLOW` when a valid payload exceeds the 256-byte static buffer). Message: `"{namespace} {root}: libcbv2g rc={status}"`.
- Raises `EncodeError` when the C++ JSON layer rejects the input — `json::parse` failure on the serialized dict, marshaler `operator[]` on a missing required field, marshaler `template get<T>()` on a wrong-typed value. Message: `"{namespace} {root}: invalid JSON input (rc=-1000)"`.
- Raises `NotImplementedError` if the Processor's schema does not define the requested root (`EXIProcessor.py`).
- Propagates `TypeError` / `ValueError` from `json.dumps` when the input is not JSON-serializable. Caller-bug errors at the Python boundary, not contract failures.

**decode (`decode`, `decode_fragment`, `decode_xmldsig`)**

- Returns a `dict` in the EVerest JSON shape on well-formed input.
- Raises `DecodeError` when libcbv2g returns a non-zero status (e.g., empty bitstream, malformed EXI header). Message names the Namespace and root.
- Raises `DecodeError` if any residual marshaler-side exception escapes during `getJson_*` (same sentinel rc as encode). The on-decode marshaler path is rarely exercised by untrusted input because it operates on a populated C struct, but the contract is uniform with encode.
- On bytes that libcbv2g accepts as well-formed but that are not actually valid for the schema, the dict content is **undefined** — may be empty, may be partially populated. The "no fixed-point on garbage" property still holds for the negative-input smoke layer.
- Raises `NotImplementedError` if the Processor's schema does not define the requested root.

**Untrusted-input isolation.** With both surfaces in-process, consumers no longer need subprocess isolation to guard against native aborts. The roundtrip subprocess in `tests/integration/conftest.py` is retained as regression-detection scaffolding, not as a safety requirement.

## Considered Options

- **Keep `bytes | None` on encode.** Rejected once the RC path landed: `None` carries no information about which Namespace / root failed; `EncodeError` carries both plus the libcbv2g rc. Two failure shapes (None vs exception) for one concept is friction with no benefit. AcCCS compat was a non-issue per project owner.
- **Add a Python-side schema validator before encode.** Still rejected: duplicates the schema knowledge that already lives in the generated marshalers, and ADR-0005's schema-driven fixture generator is the authoritative source for what a valid payload looks like. Sharing that machinery with a pre-flight validator is a larger refactor — out of scope here and tracked under #21.
- **Wrap decode with a guard that catches native aborts.** #20 chose the cheaper path: read libcbv2g's return code (the cpp wrappers were discarding it). #21 then closed the remaining marshaler surface in-process via the `JSON_ASSERT`-override + `extern "C"` try/catch approach below; see the "Considered for #21" section.

### Considered for #21 (marshaler-side hardening)

- **Forked-worker guard inside `EXIProcessor.decode_*` / `encode_*`.** Rejected: per-call latency and broken tracebacks (`with pytest.raises` doesn't survive the IPC boundary), and the cost compounds in tight encode/decode loops typical of EVerest tooling.
- **Pre-flight Python sanity checks** (EXI header probe before decode, JSON-shape probe before encode). Rejected: duplicates the schema knowledge that already lives in the generated marshalers, and only catches a subset of inputs.
- **Codegen-level `.contains()` / `.is_<T>()` guards in the marshalers.** Equivalent safety, but spreads the policy across thousands of generated lines and grows the codegen surface. The chosen JSON_ASSERT-override + outer try/catch achieves the same effect with a single line in `include/common.hpp` and one `try { ... } catch (...) { ... }` block per `extern "C"` entry point — same coverage, far less code.

## Consequences

- The negative-test smoke layer (#17) still pins the *weak* contract (the rejection envelope) under subprocess isolation; it remains valuable as defense-in-depth, but the contract floor it tests is now redundant with the strong contract. `test_decode_errors.py` and `test_encode_errors.py` pin the *strong* contract — both libcbv2g RC and marshaler-side input errors raise a typed exception with namespace + root in the message.
- The public API docs (#8) should be updated alongside this hardening: encode is now `bytes` (not `bytes | None`), raising `EncodeError` on libcbv2g failure or marshaler-input failure; decode raises `DecodeError` in the same shape. The "may abort on malformed input" warning is removed — callers can encode/decode untrusted input in-process.
