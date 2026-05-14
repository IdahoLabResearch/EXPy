# Generate Marshalers from libcbv2g Datatypes Headers

DIN and ISO‑2 are currently ~4k LoC each of hand-written `getJson_*` / `setDoc_*` Marshalers. Extending the same approach to ISO‑20's five Namespaces (Common, AC, DC, WPT, ACDP) would add ~15–25k LoC of similar boilerplate. We are introducing a Python code generator that consumes `libcbv2g`'s `*_Datatypes.h` headers and emits all Marshalers for every Processor, including regenerated DIN and ISO‑2 (replacing the hand-written code). Motivation: the marshaler shape is mechanical, the surface area is large, and the hand-written DIN code has bugs (missing presence checks on optional fields, crashing the DIN test suite) that disappear when the emitter consistently gates on `_isUsed` and `.contains()`.

## Considered Options

- **Hand-write each ISO‑20 service** following the DIN/ISO‑2 pattern. Rejected: rules out fixing the DIN bugs cheaply, multiplies maintenance, and biases against future schema bumps.
- **Generate from the XSD schemas** instead of headers. Rejected: XSDs aren't currently in-tree, and the generator would have to re-implement libcbv2g's own type-projection logic (struct layout, `_isUsed` bitfields, choice flattening). Sourcing from anything other than the headers risks drifting from libcbv2g's actual C ABI.
- **Hybrid: codegen for ISO‑20, leave hand-written DIN/ISO‑2 alone.** Rejected: two patterns to maintain forever, and the DIN bugs stay.

## Consequences

- Existing `src/DINProcessor.cpp` and `src/ISO2Processor.cpp` Marshaler bodies are deleted and regenerated. The non-Marshaler parts (CLI entry, `encodeFromPythonDict` / `decodeFromPythonBytes`, `free_encoded_data`) are kept hand-written or templated separately.
- The generator becomes a build dependency. It runs as a step in the `Makefile` before C++ compilation.
- libcbv2g header changes (from upstream submodule bumps) propagate by re-running the generator — no per-type hand-edits to chase.
- The DIN test suite is expected to pass once regenerated; "DIN testing not working" is no longer a separate task.
