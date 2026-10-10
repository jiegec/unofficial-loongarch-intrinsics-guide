// s_cmp_lt_i32 -- Compares two signed 32-bit scalar values: `d = (a < b) ? 0xffff : 0`.
//

dst[0] = cmp_lt_i32(src[0], src[1]);
