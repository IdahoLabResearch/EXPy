"""Processors whose schema doesn't define Fragment / XmldsigFragment must
expose the Python methods but raise NotImplementedError at call time.

Hides the capability difference behind a uniform surface so callers can write
``hasattr(p, "encode_fragment")``-free code that fails predictably.
"""

import multiprocessing as mp

import pytest


def _call_in_child(protocol_name, method_name, queue):
    try:
        from EXIProcessor import EXIProcessor, ProtocolEnum

        processor = EXIProcessor(ProtocolEnum[protocol_name])
        method = getattr(processor, method_name)
        try:
            method(b"" if "decode" in method_name else {})
            queue.put(("ok", None))
        except NotImplementedError as e:
            queue.put(("notimpl", str(e)))
        except Exception as e:
            queue.put(("other", f"{type(e).__name__}: {e}"))
    except Exception as e:
        queue.put(("setup_error", f"{type(e).__name__}: {e}"))


@pytest.mark.parametrize("protocol_name", ["SAP", "DIN", "ISO20_WPT", "ISO20_ACDP"])
@pytest.mark.parametrize(
    "method_name",
    ["encode_fragment", "decode_fragment", "encode_xmldsig", "decode_xmldsig"],
)
def test_fragment_and_xmldsig_methods_raise_on_unsupported_processors(
    protocol_name, method_name
):
    ctx = mp.get_context("fork")
    queue = ctx.Queue()
    proc = ctx.Process(
        target=_call_in_child, args=(protocol_name, method_name, queue)
    )
    proc.start()
    proc.join()
    assert proc.exitcode == 0, f"child aborted (exitcode={proc.exitcode})"
    status, msg = queue.get()
    assert status == "notimpl", f"expected NotImplementedError, got {status}: {msg}"
