// v_mul24_imm_i32 -- Multiplies the low 24 bits of the two signed 32-bit integer sources and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the final arithmetic or logical operand; the predicate gates the lane write.
dst[0] = mul24_i32(src[0], src[1]);
