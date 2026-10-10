// v_cmp_imm_lt_i16 -- Compares each 16-bit signed lane with the literal for `a < b`; writes a lane mask.
//

dst[0] = cmp_lt_i16(src[0], src[1]);
