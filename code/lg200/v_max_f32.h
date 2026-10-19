// v_max_f32 -- Selects the greater of the two 32-bit floating-point sources and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_max(src[0], src[1], policy);
