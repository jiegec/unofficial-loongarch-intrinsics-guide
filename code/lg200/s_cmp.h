// s_cmp -- Compares two 32-bit scalar values for equality: `d = (a == b) ? 0xffff : 0`.
//

dst[0] = cmp_eq_u32(src[0], src[1]);
