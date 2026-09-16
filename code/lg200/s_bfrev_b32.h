// s_bfrev_b32 -- Reverses the bit order of the 32-bit source and writes
// the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = bfrev_b32(src[0]);
