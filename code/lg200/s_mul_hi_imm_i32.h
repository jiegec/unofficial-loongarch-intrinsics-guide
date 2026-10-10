// s_mul_hi_imm_i32 -- Multiplies a scalar source by a signed 32-bit literal and writes the upper product half: `d = (a * b) >> 32`.
//

dst[0] = mul_hi_i32(src[0], src[1]);
