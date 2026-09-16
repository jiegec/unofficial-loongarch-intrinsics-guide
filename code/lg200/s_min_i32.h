// s_min_i32 -- Selects the lesser of the two signed 32-bit integer
// sources and writes the result to the scalar destination.
// The scalar ALU form also updates SCC as a side effect; only the value
// result is modeled here.
dst[0] = min_i32(src[0], src[1]);
