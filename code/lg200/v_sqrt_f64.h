// v_sqrt_f64 -- Computes the f64 square root: `d = sqrt(a)`.
//

u64 r = f64_sqrt(((u64)src[0] | ((u64)src[0+1] << 32)), policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
