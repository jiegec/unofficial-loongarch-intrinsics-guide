// v_fcvt_f32_i32 -- Converts the signed 32-bit integer source to a 32-bit floating-point value and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = i32_f32(src[0], policy);
