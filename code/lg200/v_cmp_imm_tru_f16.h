// v_cmp_imm_tru_f16 -- Always-true compare of f16 elements against the literal; writes a lane mask.
//

dst[0] = cmp_tru(src[0], src[1]);
