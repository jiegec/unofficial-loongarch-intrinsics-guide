// v_mul_f64 -- Multiplies two f64 values: `d = a * b`.
//

u64 r = f64_mul(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
