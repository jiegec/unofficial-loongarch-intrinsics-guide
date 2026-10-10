// s_cmp_ne_i32 -- Compares two 32-bit scalar values for inequality: `d = (a != b) ? 0xffff : 0`.
//

dst[0] = cmp_ne_u32(src[0], src[1]);
