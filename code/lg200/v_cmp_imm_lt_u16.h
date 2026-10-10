// v_cmp_imm_lt_u16 -- Compares each 16-bit unsigned lane with the literal for `a < b`; writes a lane mask.
//

dst[0] = cmp_lt_u16(src[0], src[1]);
