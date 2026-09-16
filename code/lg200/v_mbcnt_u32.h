// v_mbcnt_u32 -- Adds the population count selected by the lane mask to the accumulator source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = mbcnt_u32(src[0], src[1]);
