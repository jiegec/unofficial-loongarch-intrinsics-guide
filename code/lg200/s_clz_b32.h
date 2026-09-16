// s_clz_b32 -- Counts leading zero bits in the 32-bit source and writes
// the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = v_clz_u32(src[0]);
