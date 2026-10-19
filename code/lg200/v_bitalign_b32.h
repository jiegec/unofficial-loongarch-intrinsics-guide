// v_bitalign_b32 -- Concatenates the two 32-bit sources and extracts a bit-aligned 32-bit window and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = bitalign_b32(src[0], src[1], src[2]);
