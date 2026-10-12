// v_cmp_lt_f32 -- Compares two f32 values for less-than and writes a lane mask: `d = (a < b) ? 0xffff : 0x0000`.
//

dst[0] = cmp_lt_f32(src[0], src[1], policy);
