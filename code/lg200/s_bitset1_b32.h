// s_bitset1_b32 -- Sets the selected bit in the 32-bit source and writes
// the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here (bit selector = src[1] & 31).
dst[0] = src[0] | (1u << (src[1] & 31u));
