// s_mul_imm_u32 -- Multiplies the two unsigned 32-bit integer sources and
// writes the low result bits to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
// The product wraps at 32 bits.
dst[0] = mul_u32(src[0], src[1]);
