// v_ctz_u32 -- Counts trailing zero bits in the unsigned 32-bit integer source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = ctz_u32(src[0]);
