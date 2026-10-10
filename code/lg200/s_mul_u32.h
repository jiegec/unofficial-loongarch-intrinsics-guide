// s_mul_u32 -- Multiplies two unsigned 32-bit values and writes the low half: `d = mul_u64(a, b)[31:0]`.
//

dst[0] = mul_u32(src[0], src[1]);
