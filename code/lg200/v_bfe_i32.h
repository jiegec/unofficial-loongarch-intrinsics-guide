// v_bfe_i32 -- Extracts the bit field selected by the offset and width operands and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = bfe_i32(src[0], src[1], src[2]);
