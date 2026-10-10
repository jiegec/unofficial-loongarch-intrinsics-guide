// v_fcvt_f16_u16 -- Converts the low 16 bits of each lane's source, read as unsigned, to binary16: `d = f16(u16(a))`.
//

dst[0] = fcvt_f16_u16(src[0]);
