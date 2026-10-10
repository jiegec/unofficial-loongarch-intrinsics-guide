// v_fcvt_f32_u81 -- Converts byte 1 of each lane's source, read as unsigned, to single precision: `d = f32(u8(a[15:8]))`.
//

dst[0] = u32_f32((src[0] >> 8) & 0xffu, policy);
