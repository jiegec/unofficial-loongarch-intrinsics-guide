// v_bfrev_b32 -- Reverses the bit order of the 32-bit source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = bfrev_b32(src[0]);
