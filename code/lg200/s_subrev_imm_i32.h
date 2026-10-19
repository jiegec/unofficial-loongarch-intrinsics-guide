// s_subrev_imm_i32 -- Subtracts the first source from the second source
// and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = src[1] - src[0];
