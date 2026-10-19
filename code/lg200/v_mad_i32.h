// v_mad_i32 -- Multiplies the first two integer sources and adds the third source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = mad_i32(src[0], src[1], src[2]);
