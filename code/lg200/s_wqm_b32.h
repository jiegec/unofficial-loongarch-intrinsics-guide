// s_wqm_b32 -- Expands each nonzero four-bit source group to an all-one
// four-bit mask and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = wqm(src[0]);
