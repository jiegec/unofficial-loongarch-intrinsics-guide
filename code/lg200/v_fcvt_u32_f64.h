// v_fcvt_u32_f64 -- Converts each lane's f64 source to an unsigned 32-bit integer: `d = u32(a)`.
//

dst[0] = f64_u32(((u64)src[0] | ((u64)src[0+1] << 32)), policy);
