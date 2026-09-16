// s_sllrev_imm_b32 -- Shifts the second source left by the first-source
// shift count and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
// Reversed-operand form: count = src[0], data = src[1] (count masked to
// the low 5 bits).
dst[0] = src[1] << (src[0] & 31u);
