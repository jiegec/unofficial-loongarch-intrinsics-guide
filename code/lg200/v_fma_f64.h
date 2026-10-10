// v_fma_f64 -- Computes a fused multiply-add of three f64 sources: `d = a * b + c`.
//

u64 r = f64_fma(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), ((u64)src[4] | ((u64)src[4+1] << 32)), policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
