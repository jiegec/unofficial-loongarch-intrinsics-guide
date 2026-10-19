// v_abs_i32 -- Computes the absolute value of the signed 32-bit integer source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = (src[0] & 0x80000000u) == 0 ? src[0] : 0u - src[0];
