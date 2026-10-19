// s_mul_hi_imm_u32 -- Multiplies the two unsigned 32-bit integer sources
// and writes the high half of the full product to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = mul_hi_u32(src[0], src[1]);
