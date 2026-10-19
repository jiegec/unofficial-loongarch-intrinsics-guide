// v_min3_i32 -- Selects the least of the three sources and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = min3_i32(src[0], src[1], src[2]);
