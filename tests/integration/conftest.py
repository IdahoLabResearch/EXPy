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


def _roundtrip_worker(protocol_name, payload_json, queue):
    # Runs in a child process so a native abort doesn't kill the test runner.
    try:
        from EXIProcessor import EXIProcessor, ProtocolEnum

        processor = EXIProcessor(ProtocolEnum[protocol_name])
        payload = json.loads(payload_json)
        exi = processor.encode(payload)
        if exi is None:
            queue.put(("fail", "encode() returned None"))
            return
        decoded = processor.decode(exi)
        if _ordered(decoded) != _ordered(payload):
            queue.put(("fail", f"roundtrip mismatch: decoded={decoded!r}"))
            return
        queue.put(("ok", None))
    except Exception as e:
        queue.put(("fail", f"{type(e).__name__}: {e}"))


def assert_roundtrip(protocol_name, payload):
    """Encode/decode `payload` in a forked process; assert the roundtrip matches.

    Isolating in a child process prevents native aborts in libcbv2g from
    crashing the pytest runner — we surface them as ordinary test failures.
    """
    ctx = mp.get_context("fork")
    queue = ctx.Queue()
    proc = ctx.Process(
        target=_roundtrip_worker,
        args=(protocol_name, json.dumps(payload), queue),
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
