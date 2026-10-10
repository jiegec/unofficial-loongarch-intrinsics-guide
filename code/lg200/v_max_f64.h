// v_max_f64 -- Selects the greater of two f64 values, one register pair per lane: `d = max(a, b)`.
//

u64 r = f64_max(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)));
dst[0] = (u32)(r & 0xffffffffu);
dst[1] = (u32)(r >> 32);
