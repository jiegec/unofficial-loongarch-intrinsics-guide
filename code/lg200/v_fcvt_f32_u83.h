// v_fcvt_f32_u83 -- Converts byte 3 of each lane's source, read as unsigned, to single precision: `d = f32(u8(a[31:24]))`.
//

dst[0] = fcvt_f32_u83(src[0]);
