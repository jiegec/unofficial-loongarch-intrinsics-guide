// v_bfm_b32 -- Constructs a contiguous bit mask from the offset and width operands and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = bfm_b32(src[0], src[1]);
