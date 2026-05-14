import json
import multiprocessing as mp
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO_ROOT))


def _ordered(obj):
    if isinstance(obj, dict):
        return sorted((k, _ordered(v)) for k, v in obj.items())
    if isinstance(obj, list):
        return sorted(_ordered(x) for x in obj)
    return obj


_ENCODE_DECODE = {
    "document": ("encode", "decode"),
    "fragment": ("encode_fragment", "decode_fragment"),
    "xmldsig": ("encode_xmldsig", "decode_xmldsig"),
}


def _roundtrip_worker(protocol_name, kind, payload_json, queue):
    # Runs in a child process so a native abort doesn't kill the test runner.
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
        if _ordered(decoded) != _ordered(payload):
            queue.put(("fail", f"roundtrip mismatch: decoded={decoded!r}"))
            return
        queue.put(("ok", None))
    except Exception as e:
        queue.put(("fail", f"{type(e).__name__}: {e}"))


def assert_roundtrip(protocol_name, payload, kind="document"):
    """Encode/decode `payload` in a forked process; assert the roundtrip matches.

    Isolating in a child process prevents native aborts in libcbv2g from
    crashing the pytest runner — we surface them as ordinary test failures.

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
    proc.join()

    if proc.exitcode != 0:
        if not queue.empty():
            _, msg = queue.get_nowait()
            raise AssertionError(f"child aborted (exitcode={proc.exitcode}): {msg}")
        raise AssertionError(f"child aborted (exitcode={proc.exitcode})")

    status, msg = queue.get()
    assert status == "ok", msg
