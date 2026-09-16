// s_bfm_b32 -- Constructs a contiguous bit mask from the offset and width
// operands and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = bfm_b32(src[0], src[1]);
