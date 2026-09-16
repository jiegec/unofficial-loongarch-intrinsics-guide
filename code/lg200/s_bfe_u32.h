// s_bfe_u32 -- Extracts the bit field selected by the offset and width
// operands and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = bfe_u32(src[0], src[1], src[2]);
