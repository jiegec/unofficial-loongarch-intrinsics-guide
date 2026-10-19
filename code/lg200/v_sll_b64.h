// v_sll_b64 -- Shifts the 64-bit source left by the masked shift count and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
u64 r = ((u64)src[2] | ((u64)src[2+1] << 32)) < 64 ? ((u64)src[0] | ((u64)src[0+1] << 32)) << (((u64)src[2] | ((u64)src[2+1] << 32)) & 63u) : 0;
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
