// v_cos_f32 -- Computes the cosine of the 32-bit floating-point source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_cos(src[0], policy);
