// v_cmp_imm_lt_u32 -- Compares each 32-bit unsigned lane with the literal for `a < b`; writes a lane mask.
//

dst[0] = cmp_lt_u32(src[0], src[1]);
