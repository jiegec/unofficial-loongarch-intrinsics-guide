// s_mul_hi_u32 -- Multiplies two unsigned 32-bit values and writes the high half: `d = mul_u64(a, b)[63:32]`.
//

dst[0] = mul_hi_u32(src[0], src[1]);
