// v_cmp_imm_lt_f32 -- Compares each f32 lane with the literal for `a < b`; writes a lane mask.
//

dst[0] = cmp_lt_f32(src[0], src[1], policy);
