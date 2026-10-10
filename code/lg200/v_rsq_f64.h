// v_rsq_f64 -- Approximates the reciprocal square root of an f64 value as a two-register result.
//

u64 r = f64_rsq(((u64)src[0] | ((u64)src[0+1] << 32)), policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
