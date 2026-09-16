// s_abs_i32 -- Computes the absolute value of the signed 32-bit integer
// source and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = (src[0] & 0x80000000u) == 0 ? src[0] : 0u - src[0];
