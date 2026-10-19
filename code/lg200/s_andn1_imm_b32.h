// s_andn1_imm_b32 -- Computes the bitwise AND of the complemented first
// source and the second source and writes the result to the scalar
// destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = ~src[0] & src[1];
