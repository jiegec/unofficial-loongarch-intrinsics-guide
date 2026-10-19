// v_fcvt_i32flr_f32 -- Converts the 32-bit floating-point source to signed 32-bit integer with floor rounding and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = fcvt_i32flr_f32(src[0]);
