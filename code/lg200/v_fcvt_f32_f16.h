// v_fcvt_f32_f16 -- Widens each lane's half-precision source to single precision: `d = f32(f16(a))`.
//

dst[0] = f16_f32(src[0], policy);
