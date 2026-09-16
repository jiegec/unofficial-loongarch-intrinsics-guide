// v_sub_imm_u32 -- Subtracts the second source from the first source and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = sub_u32_value(src[0], src[1]);
