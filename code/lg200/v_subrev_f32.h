// v_subrev_f32 -- Subtracts the first source from the second source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = f32_revsub(src[0], src[1], policy);
