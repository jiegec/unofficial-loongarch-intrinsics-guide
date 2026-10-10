// v_cmp_imm_neq_f16 -- Compares each f16 lane with the literal for inequality, unordered included; writes a lane mask.
//

dst[0] = cmp_neq_f16(src[0], src[1]);
