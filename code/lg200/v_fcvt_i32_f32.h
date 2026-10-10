// v_fcvt_i32_f32 -- Converts each lane's single-precision source to the nearest signed 32-bit integer: `d = i32(a)`.
//

dst[0] = f32_i32(src[0], policy);
