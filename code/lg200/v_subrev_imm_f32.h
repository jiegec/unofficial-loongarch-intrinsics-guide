// v_subrev_imm_f32 -- Subtracts the first source from the second source and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = f32_revsub(src[0], src[1], policy);
