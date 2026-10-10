// gather_c_b -- Depth-compare gather into the aligned VD8x4 group at the implicit level plus an LOD bias `lambda = log2(rho) + bias`.
//
// observed mode: the runner cannot stage texture launch state, so
// this row records that it ran and what was seen rather than a comparison against
// an independent oracle.  What it established, and what it leaves
// UNRESOLVED, is stated in the `**Observed.**` and `**Boundaries.**`
// sections of this row in `code/lg200-instructions.tsv`.

dst[0] = src[0];
