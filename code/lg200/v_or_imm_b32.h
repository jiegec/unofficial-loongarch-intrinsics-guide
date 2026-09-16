// v_or_imm_b32 -- Computes the bitwise OR of the two 32-bit sources and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = src[0] | src[1];
