// s_sra_b32 -- Arithmetically shifts the signed source right by the
// masked shift count and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here (count is masked to the low 5 bits).
dst[0] = sra_b32(src[0], src[1]);
