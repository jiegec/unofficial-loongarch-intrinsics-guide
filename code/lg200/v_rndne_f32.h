// v_rndne_f32 -- Rounds the 32-bit floating-point source to the nearest integral value with ties to even and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_rndne(src[0], policy);
