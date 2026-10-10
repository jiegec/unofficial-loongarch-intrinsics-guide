// gather_c_b_o -- Depth-compare gather into the aligned VD8x4 group with an LOD bias and a offset.
// The gather offset is not an instruction operand: the `_o` opcode selects a tuple layout whose
// leading slot carries it, packed as `x[5:0] | (y[13:8] << 8)` (the same packing as samp*_o) and
// read from the `VJ` register base; every other slot shifts up by one (see docs/gpu/memory-image.md).
//
// observed mode: the runner cannot stage texture launch state, so
// this row records that it ran and what was seen rather than a comparison against
// an independent oracle.  What it established, and what it leaves
// UNRESOLVED, is stated in the `**Observed.**` and `**Boundaries.**`
// sections of this row in `code/lg200-instructions.tsv`.

dst[0] = src[0];
