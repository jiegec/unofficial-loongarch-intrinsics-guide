// v_sub_imm_f32 -- Subtracts the second source from the first source and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = f32_sub(src[0], src[1], policy);
