// v_fcvt_i32_f32 -- Converts the 32-bit floating-point value source to a signed 32-bit integer and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_i32(src[0], policy);
