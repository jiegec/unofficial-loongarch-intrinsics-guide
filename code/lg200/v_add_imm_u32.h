// v_add_imm_u32 -- Adds the two unsigned 32-bit integer sources and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the second operand; dst[0] is the sum and dst[1] carries the carry-out. The predicate gates the lane write.
u32 sum = src[0] + src[1];
dst[0] = sum;
dst[1] = (sum < src[0]) ? 1u : 0u;
