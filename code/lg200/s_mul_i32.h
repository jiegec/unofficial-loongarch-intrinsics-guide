// s_mul_i32 -- Multiplies the two signed 32-bit integer sources and
// writes the low result bits to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here (the low 32 bits are identical for i32/u32).
dst[0] = mul_u32(src[0], src[1]);
