// v_fcvt_i32_f64 -- Converts each lane's double-precision source to the nearest signed 32-bit integer: `d = i32(a)`.
//

dst[0] = f64_i32(((u64)src[0] | ((u64)src[0+1] << 32)), policy);
