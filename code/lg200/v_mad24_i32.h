// v_mad24_i32 -- Multiplies the low 24-bit signed 32-bit integer source values and adds the third source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = mad24_i32(src[0], src[1], src[2]);
