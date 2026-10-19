// v_xnor_b32 -- Computes the complement of the bitwise XOR of the two 32-bit sources and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = ~(src[0] ^ src[1]);
