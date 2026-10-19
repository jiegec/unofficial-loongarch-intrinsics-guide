// s_add_u32 -- Adds the two unsigned 32-bit integer sources and writes the
// result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here (the sum wraps at 32 bits).
dst[0] = src[0] + src[1];
