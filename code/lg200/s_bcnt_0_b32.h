// s_bcnt_0_b32 -- Counts zero bits in the 32-bit source and writes the
// result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = bcnt0_u32(src[0]);
