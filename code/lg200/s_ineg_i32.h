// s_ineg_i32 -- Computes the two's-complement negation of the signed
// 32-bit integer source and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = 0u - src[0];
