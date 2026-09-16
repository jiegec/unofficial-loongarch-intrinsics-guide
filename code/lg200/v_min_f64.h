// v_min_f64 -- Selects the lesser of the two 64-bit floating-point sources and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
u64 r = f64_min(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
