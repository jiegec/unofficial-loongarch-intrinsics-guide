// v_trunc_f64 -- Rounds the 64-bit floating-point source toward zero to an integral value and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
u64 r = f64_trunc(((u64)src[0] | ((u64)src[0+1] << 32)), policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
