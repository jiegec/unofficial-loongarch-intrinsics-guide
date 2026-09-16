// v_min_i32 -- Selects the lesser of the two signed 32-bit integer sources and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = min_i32(src[0], src[1]);
