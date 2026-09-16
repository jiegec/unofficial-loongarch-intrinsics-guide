// v_byteperm_b32 -- Selects and permutes source bytes according to the control operand and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = byteperm_b32(src[0], src[1], src[2]);
