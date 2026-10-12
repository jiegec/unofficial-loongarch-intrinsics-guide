// v_cmp_imm_lt_f16 -- Compares each f16 lane with the literal for `a < b`; writes a lane mask.
//

dst[0] = cmp_lt_f16(src[0], src[1], policy);
