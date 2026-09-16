// v_sra_b64 -- Arithmetically shifts the signed source right by the masked shift count and writes the result to the vector destination
// The predicate gates the lane write; only the value result of the named operation is modeled here.
u64 r = sra_b64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
