// s_xor_b32 -- Computes the bitwise XOR of the two 32-bit sources and
// writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = src[0] ^ src[1];
