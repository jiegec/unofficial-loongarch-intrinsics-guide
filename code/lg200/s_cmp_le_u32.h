// s_cmp_le_u32 -- Compares two unsigned 32-bit scalar values: `d = (a <= b) ? 0xffff : 0`.
//

dst[0] = cmp_le_u32(src[0], src[1]);
