// v_max_imm_u32 -- Selects the greater of the two unsigned 32-bit integer sources and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = max_u32(src[0], src[1]);
