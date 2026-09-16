// v_sub_i32 -- Subtracts the second source from the first source and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = src[0] - src[1];
