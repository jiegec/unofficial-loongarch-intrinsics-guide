// v_sext_i32_i8 -- Sign-extends the signed 8-bit integer source to a signed 32-bit integer and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = sext_i32_i8(src[0]);
