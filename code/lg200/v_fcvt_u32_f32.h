// v_fcvt_u32_f32 -- Converts each lane's f32 source to an unsigned 32-bit integer: `d = u32(a)`.
//

dst[0] = f32_u32(src[0], policy);
