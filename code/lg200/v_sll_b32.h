// v_sll_b32 -- Shifts the 32-bit source left by the masked shift count and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = src[0] << (src[1] & 31u);
