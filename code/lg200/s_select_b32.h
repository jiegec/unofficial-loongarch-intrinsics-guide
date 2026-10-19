// s_select_b32 -- Selects one of two data sources according to the
// condition source and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here (condition = src[2] != 0).
dst[0] = src[2] != 0 ? src[0] : src[1];
