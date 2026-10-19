// v_fcvt_f64_i32 -- Converts the signed 32-bit integer source to a 64-bit floating-point value and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
u64 r = i32_f64(src[0]);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
