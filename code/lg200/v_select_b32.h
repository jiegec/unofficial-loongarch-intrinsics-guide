// v_select_b32 -- Selects one of two data sources according to the condition source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = select_b32(src[0], src[1], src[2]);
