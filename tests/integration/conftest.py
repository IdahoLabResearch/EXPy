import json
import multiprocessing as mp
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))


_ENCODE_DECODE = {
    "document": ("encode", "decode"),
    "fragment": ("encode_fragment", "decode_fragment"),
    "xmldsig": ("encode_xmldsig", "decode_xmldsig"),
}


def _roundtrip_worker(protocol_name, kind, payload_json, queue):
    # Originally needed to contain native aborts; since #21 closed the marshaler
    # abort surface, the parent process is also safe. Kept as belt-and-suspenders
    # regression scaffolding — any future surface that bypasses the in-process
    # guards will surface here as a non-zero exitcode instead of a crash.
    try:
        from EXIProcessor import EXIProcessor, ProtocolEnum

        processor = EXIProcessor(ProtocolEnum[protocol_name])
        encode_attr, decode_attr = _ENCODE_DECODE[kind]
        encode = getattr(processor, encode_attr)
        decode = getattr(processor, decode_attr)

        payload = json.loads(payload_json)
        exi = encode(payload)
        if exi is None:
            queue.put(("fail", f"{encode_attr}() returned None"))
            return
        decoded = decode(exi)
        if decoded != payload:
            queue.put(("fail", f"roundtrip mismatch: decoded={decoded!r}"))
            return
        queue.put(("ok", None))
    except Exception as e:
        queue.put(("fail", f"{type(e).__name__}: {e}"))


def assert_roundtrip(protocol_name, payload, kind="document"):
    """Encode/decode `payload` in a forked process; assert the roundtrip matches.

    Since #21 the in-process encode/decode is abort-safe (see ADR-0006), so this
    helper no longer guards against the runner crashing — it is retained as
    regression scaffolding that turns any future native abort into a non-zero
    exit code instead of a process kill.

    `kind` selects which Processor method pair to exercise: "document" (default),
    "fragment", or "xmldsig".
    """
    ctx = mp.get_context("fork")
    queue = ctx.Queue()
    proc = ctx.Process(
        target=_roundtrip_worker,
        args=(protocol_name, kind, json.dumps(payload), queue),
    )
    proc.start()
    proc.join(timeout=15)
    if proc.is_alive():
        proc.terminate()
        proc.join(timeout=2)
        if proc.is_alive():
            proc.kill()
            proc.join()
        raise AssertionError("child timed out (encoder/decoder did not return)")

    if proc.exitcode != 0:
        if not queue.empty():
            _, msg = queue.get_nowait()
            raise AssertionError(f"child aborted (exitcode={proc.exitcode}): {msg}")
        raise AssertionError(f"child aborted (exitcode={proc.exitcode})")

    status, msg = queue.get()
    assert status == "ok", msg
