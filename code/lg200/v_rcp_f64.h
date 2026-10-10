// v_rcp_f64 -- Produces an approximation of the reciprocal of an f64 value as a two-register result: `d = 1/a`.
//

u64 r = f64_rcp(((u64)src[0] | ((u64)src[0+1] << 32)), policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
