// s_mul_imm_u32 -- Multiplies a scalar source by an unsigned 32-bit literal and writes the low 32 bits of the product: `d = (a * b)[31:0]`.
//

dst[0] = mul_u32(src[0], src[1]);
