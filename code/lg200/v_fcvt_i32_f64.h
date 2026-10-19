// v_fcvt_i32_f64 -- Converts the 64-bit floating-point value source to a signed 32-bit integer and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f64_i32(((u64)src[0] | ((u64)src[0+1] << 32)), policy);
