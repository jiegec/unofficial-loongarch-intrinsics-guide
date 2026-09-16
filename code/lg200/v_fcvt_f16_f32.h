// v_fcvt_f16_f32 -- Converts the 32-bit floating-point value source to a 16-bit floating-point value and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_f16(src[0], policy);
