// v_cmp_lt_i32 -- Compares two signed 32-bit integers for less-than and writes a lane mask: `d = (a < b) ? 0xffff : 0x0000`.
//

dst[0] = cmp_lt_i32(src[0], src[1]);
