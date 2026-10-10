// s_mul_imm_i32 -- Multiplies a scalar by a signed 32-bit literal and writes the low product half: `d = (a * b)[31:0]`.
//

dst[0] = mul_u32(src[0], src[1]);
