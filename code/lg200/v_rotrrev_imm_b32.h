// v_rotrrev_imm_b32 -- Rotates the second source right by the first-source shift count and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = rotrrev_b32(src[0], src[1]);
