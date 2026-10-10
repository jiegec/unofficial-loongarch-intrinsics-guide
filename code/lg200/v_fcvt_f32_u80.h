// v_fcvt_f32_u80 -- Converts byte 0 of each lane's source, read as unsigned, to single precision: `d = f32(u8(a[7:0]))`.
//

dst[0] = fcvt_f32_u80(src[0]);
