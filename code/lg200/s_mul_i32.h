// s_mul_i32 -- Multiplies two signed 32-bit values and writes the low half: `d = mul_i64(a, b)[31:0]`.
//

dst[0] = mul_u32(src[0], src[1]);
