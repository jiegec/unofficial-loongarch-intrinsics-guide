// s_mul_hi_imm_u32 -- Multiplies a scalar by an unsigned 32-bit literal and writes the upper product half: `d = (a * b) >> 32`.
//

dst[0] = mul_hi_u32(src[0], src[1]);
