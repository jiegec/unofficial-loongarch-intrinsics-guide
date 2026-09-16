// v_ldexp_f32 -- Scales the floating-point significand by two raised to the integer exponent and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_ldexp(src[0], src[1], policy);
