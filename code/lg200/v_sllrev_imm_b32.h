// v_sllrev_imm_b32 -- Shifts the second source left by the first-source shift count and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = src[1] << (src[0] & 31u);
