// v_absdiff_u32 -- Computes the absolute difference of the two unsigned 32-bit integer sources and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = src[0] >= src[1] ? src[0] - src[1] : src[1] - src[0];
