// s_mul_hi_i32 -- Multiplies two signed 32-bit values and writes the high half: `d = mul_i64(a, b)[63:32]`.
//

dst[0] = mul_hi_i32(src[0], src[1]);
