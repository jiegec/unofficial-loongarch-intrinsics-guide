// s_cto_b32 -- Counts trailing one bits in the 32-bit source and writes
// the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = cto_u32(src[0]);
