// v_fcvt_f32_u32 -- Converts each lane's unsigned 32-bit integer source to single precision: `d = f32(u32(a))`.
//

dst[0] = u32_f32(src[0], policy);
