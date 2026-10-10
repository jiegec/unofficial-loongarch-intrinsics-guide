// v_fcvt_f16_i16 -- Converts the low 16 bits of each lane's source, read as signed, to binary16: `d = f16(i16(a))`.
//

dst[0] = fcvt_f16_i16(src[0]);
