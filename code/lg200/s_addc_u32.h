// s_addc_u32 -- Adds the two unsigned 32-bit integer sources and the low
// carry-in bit and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here (src[2][0] is the carry-in).
dst[0] = s_addc_u32(src[0], src[1], src[2]);
