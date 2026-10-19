// s_srl_b32 -- Logically shifts the 32-bit source right by the masked
// shift count and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here (count is masked to the low 5 bits).
dst[0] = src[0] >> (src[1] & 31u);
