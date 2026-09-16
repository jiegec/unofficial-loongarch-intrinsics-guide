// v_not_b32 -- Computes the bitwise complement of the 32-bit source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = ~src[0];
