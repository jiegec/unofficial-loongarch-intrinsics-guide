// v_fcvt_f32_u82 -- Converts byte 2 of the 32-bit source as an unsigned 8-bit integer to 32-bit floating point and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = fcvt_f32_u82(src[0]);
