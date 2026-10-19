// v_max_u32 -- Selects the greater of the two unsigned 32-bit integer sources and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = max_u32(src[0], src[1]);
