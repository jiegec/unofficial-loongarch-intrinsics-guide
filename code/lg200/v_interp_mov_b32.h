// v_interp_mov_b32 -- Copies the selected 32-bit interpolation payload value to the active fragment lane's vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
dst[0] = src[0];
