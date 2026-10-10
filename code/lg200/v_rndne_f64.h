// v_rndne_f64 -- Rounds an f64 value to the nearest integral value with ties to even.
//

u64 r = f64_rndne(((u64)src[0] | ((u64)src[0+1] << 32)), policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
