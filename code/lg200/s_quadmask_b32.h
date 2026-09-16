// s_quadmask_b32 -- Reduces each four-bit group of the 32-bit source to
// one nonzero mask bit and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = quadmask(src[0]);
