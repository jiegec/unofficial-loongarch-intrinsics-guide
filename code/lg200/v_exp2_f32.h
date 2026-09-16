// v_exp2_f32 -- Computes two raised to the 32-bit floating-point source value and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_exp2_(src[0], policy);
