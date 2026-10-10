// v_mad24_i32 -- Multiplies the low 24-bit signed fields of two sources and adds the third: `d = (s24)a * (s24)b + c`.
//

dst[0] = mad24_i32(src[0], src[1], src[2]);
