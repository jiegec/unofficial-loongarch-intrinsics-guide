// v_ldexp_imm_f32 -- Scales the floating-point significand by two raised to the integer exponent and writes the result to the vector destination. The encoded immediate supplies the final arithmetic or logical source operand. The predicate gates the lane write.
// The encoded immediate supplies the exponent; the predicate gates the lane write.
dst[0] = f32_ldexp(src[0], src[1], policy);
