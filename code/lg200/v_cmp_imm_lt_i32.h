// v_cmp_imm_lt_i32 -- Compares each 32-bit signed lane with the literal for `a < b`; writes a lane mask.
//

dst[0] = cmp_lt_i32(src[0], src[1]);
