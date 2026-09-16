// v_copysign_imm_f32 -- Combines the magnitude of the first source with the sign of the second source and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = copysign_imm_f32(src[0], src[1], policy);
