// v_srarev_imm_b32 -- Arithmetically shifts the signed second source right by the first-source shift count and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = srarev_b32(src[0], src[1]);
