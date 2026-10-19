// v_s2v_b32 -- Broadcasts the selected 32-bit scalar source value to the vector destination of each active lane
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = src[0];
