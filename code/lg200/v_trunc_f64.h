// v_trunc_f64 -- Rounds each f64 lane toward zero to an integral value: `d = trunc(a)`.
//

u64 r = f64_trunc(((u64)src[0] | ((u64)src[0+1] << 32)), policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
