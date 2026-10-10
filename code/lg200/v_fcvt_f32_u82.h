// v_fcvt_f32_u82 -- Converts byte 2 of each lane's source, read as unsigned, to single precision: `d = f32(u8(a[23:16]))`.
//

dst[0] = fcvt_f32_u82(src[0]);
