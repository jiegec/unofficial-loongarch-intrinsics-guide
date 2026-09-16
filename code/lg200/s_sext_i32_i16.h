// s_sext_i32_i16 -- Sign-extends the signed 16-bit integer source to a
// signed 32-bit integer and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = sext_i32_i16(src[0]);
