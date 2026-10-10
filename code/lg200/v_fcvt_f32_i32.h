// v_fcvt_f32_i32 -- Converts each lane's signed 32-bit integer source to single precision: `d = f32(i32(a))`.
//

dst[0] = i32_f32(src[0], policy);
