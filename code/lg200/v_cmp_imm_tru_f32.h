// v_cmp_imm_tru_f32 -- Always-true compare of f32 elements against the literal; writes a lane mask.
//

dst[0] = cmp_tru(src[0], src[1]);
