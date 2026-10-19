// v_ceil_f32 -- Rounds the 32-bit floating-point source toward positive infinity to an integral value and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_ceil(src[0], policy);
