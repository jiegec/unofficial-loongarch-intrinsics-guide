// v_mac_imm_f32 -- Multiplies the two 32-bit floating-point sources and adds the current destination accumulator and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = f32_fma(src[0], src[1], src[2], policy);
