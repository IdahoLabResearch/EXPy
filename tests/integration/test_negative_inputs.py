"""Negative-input smoke layer (~3 per namespace).

Per ADR-0006:

- Encode of structurally invalid input (missing required nested field,
  wrong-typed scalar) may return `None`, raise, or abort the process via the
  generated marshalers' `nlohmann::json` accesses. Any of those counts as
  "rejected"; a normal return of a `bytes` buffer is a contract violation.
- Decode of malformed EXI is best-effort and the dict content is undefined,
  but the contract pins down one positive property: `encode(decode(b)) != b`
  for arbitrary bytes `b`. The smoke tests below assert that property.

All tests are subprocess-isolated using the same pattern as
`tests/integration/conftest.py:_roundtrip_worker`.
"""

import json
import multiprocessing as mp
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))


def _negative_worker(protocol_name, op, arg_json, queue):
    # Build the Processor first; only then attempt the operation under test.
    # The "ready" sentinel on the queue lets the parent tell apart a setup
    # failure (no sentinel) from a native abort *during* encode/decode.
    try:
        from expy import EXIProcessor, Namespace

        processor = EXIProcessor(Namespace[protocol_name])
    except Exception as e:
        queue.put(("setup_error", f"{type(e).__name__}: {e}"))
        return

    queue.put(("ready", None))

    try:
        if op == "encode":
            payload = json.loads(arg_json)
            result = processor.encode(payload)
            if result is None:
                queue.put(("rejected", "encode returned None"))
            else:
                queue.put(("accepted", f"encode returned bytes(len={len(result)})"))
        elif op == "decode_no_fixed_point":
            # The contract pins down one positive property on malformed
            # decode: `encode(decode(b)) != b`. Any other outcome (decode
            # raises, decode aborts, encode fails) also counts as "not a
            # fixed point" and therefore "rejected".
            buf = bytes(json.loads(arg_json))
            decoded = processor.decode(buf)
            re_encoded = processor.encode(decoded)
            if re_encoded == buf:
                queue.put(
                    ("accepted", f"fixed point reached on len={len(buf)} input")
                )
            else:
                queue.put(
                    (
                        "rejected",
                        f"decoded={decoded!r}; re-encoded len={None if re_encoded is None else len(re_encoded)}",
                    )
                )
        else:
            queue.put(("setup_error", f"unknown op {op!r}"))
    except Exception as e:
        queue.put(("rejected", f"{type(e).__name__}: {e}"))


def _run_negative(protocol_name, op, arg):
    """Run encode/decode in a forked child and classify the outcome."""
    ctx = mp.get_context("fork")
    queue = ctx.Queue()
    if op == "encode":
        arg_json = json.dumps(arg)
    else:  # decode_no_fixed_point
        arg_json = json.dumps(list(arg))  # bytes -> list[int]
    proc = ctx.Process(
        target=_negative_worker,
        args=(protocol_name, op, arg_json, queue),
    )
    proc.start()
    proc.join(timeout=15)
    if proc.is_alive():
        proc.terminate()
        proc.join(timeout=2)
        if proc.is_alive():
            proc.kill()
            proc.join()
        raise AssertionError(f"child timed out ({op} did not return)")

    messages = []
    while not queue.empty():
        messages.append(queue.get_nowait())

    ready_seen = any(m[0] == "ready" for m in messages)

    if proc.exitcode != 0:
        if not ready_seen:
            # Aborted before reaching the operation under test — setup failed.
            setup_msgs = [m for m in messages if m[0] == "setup_error"]
            detail = setup_msgs[0][1] if setup_msgs else "(no detail)"
            raise AssertionError(
                f"child aborted during setup (exitcode={proc.exitcode}): {detail}"
            )
        # Aborted during encode/decode — counts as "rejected" per ADR-0006.
        return "rejected"

    # Clean exit. Find the outcome message (skip the "ready" sentinel).
    outcomes = [m for m in messages if m[0] in ("accepted", "rejected", "setup_error")]
    if not outcomes:
        raise AssertionError(f"child exited 0 but produced no outcome: {messages!r}")
    status, _detail = outcomes[-1]
    if status == "setup_error":
        raise AssertionError(f"setup failure: {_detail}")
    return status


def _assert_encode_rejects(protocol_name, payload):
    status = _run_negative(protocol_name, "encode", payload)
    assert status == "rejected", f"expected rejection, got {status}"


def _assert_decode_garbage_is_not_a_fixed_point(protocol_name, exi_bytes):
    """The one positive guarantee on malformed decode (see ADR-0006):
    `encode(decode(b)) != b` for arbitrary bytes `b`. The implementation may
    also reject by raising or aborting; both satisfy the contract.
    """
    status = _run_negative(protocol_name, "decode_no_fixed_point", exi_bytes)
    assert status == "rejected", (
        f"garbage roundtripped as a fixed point — decode silently treated "
        f"arbitrary bytes as valid input ({status})"
    )


# ----- SAP --------------------------------------------------------------


def test_sap_encode_missing_required_field_rejects():
    # Required `AppProtocol` omitted from the request body.
    _assert_encode_rejects("SAP", {"supportedAppProtocolReq": {}})


def test_sap_encode_wrong_typed_field_rejects():
    # `arrayLen` must be an unsigned int; a string violates the typed accessor.
    _assert_encode_rejects(
        "SAP",
        {
            "supportedAppProtocolReq": {
                "AppProtocol": {"arrayLen": "not-an-int", "array": []}
            }
        },
    )


def test_sap_decode_malformed_exi_is_not_a_fixed_point():
    # Garbage bytes — not a valid EXI bitstream for SAP. Decode may return
    # *some* dict; the contract guarantees it won't be a roundtrip fixed
    # point of the garbage input.
    _assert_decode_garbage_is_not_a_fixed_point("SAP", b"\xff" * 32)


# ----- DIN --------------------------------------------------------------


def test_din_encode_missing_required_field_rejects():
    # V2G_Message requires a Header; omitting it makes the marshaler dereference
    # a missing key.
    _assert_encode_rejects("DIN", {"V2G_Message": {}})


def test_din_encode_wrong_typed_field_rejects():
    # Header.SessionID expects a {"bytes": [...], "bytesLen": N} object;
    # a bare string violates the typed accessor.
    _assert_encode_rejects(
        "DIN", {"V2G_Message": {"Header": {"SessionID": "not-bytes"}}}
    )


def test_din_decode_malformed_exi_is_not_a_fixed_point():
    _assert_decode_garbage_is_not_a_fixed_point("DIN", b"\xff" * 32)


# ----- ISO2 -------------------------------------------------------------


def test_iso2_encode_missing_required_field_rejects():
    _assert_encode_rejects("ISO2", {"V2G_Message": {}})


def test_iso2_encode_wrong_typed_field_rejects():
    _assert_encode_rejects(
        "ISO2", {"V2G_Message": {"Header": {"SessionID": "not-bytes"}}}
    )


def test_iso2_decode_malformed_exi_is_not_a_fixed_point():
    _assert_decode_garbage_is_not_a_fixed_point("ISO2", b"\xff" * 32)
