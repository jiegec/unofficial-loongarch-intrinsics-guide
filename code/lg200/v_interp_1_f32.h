// v_interp_1_f32 -- Performs the first 32-bit floating-point interpolation phase for an active fragment lane and writes the intermediate value to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = v_interp_1_f32(src[0], src[1]);
