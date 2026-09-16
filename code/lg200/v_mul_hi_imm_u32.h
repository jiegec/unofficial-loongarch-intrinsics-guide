// v_mul_hi_imm_u32 -- Multiplies the two unsigned 32-bit integer sources and writes the high half of the full product to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = mul_hi_u32(src[0], src[1]);
