// v_mad24_u32 -- Multiplies the low 24 bits of two unsigned sources and adds a third: `d = a[23:0] * b[23:0] + c`.
//

dst[0] = mad24_u32(src[0], src[1], src[2]);
