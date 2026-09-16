// s_mov_b32 -- Copies the 32-bit source value and writes the result to
// the scalar destination.
// The scalar ALU form also updates SCC as a side effect ([HW-verified]
// s_mov was the observed SCC clobber in the 2026-08 compare probes); only
// the value result is modeled here.
dst[0] = src[0];
