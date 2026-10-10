// v_cmp_imm_neq_f32 -- Compares each f32 lane with the literal for inequality, unordered included; writes a lane mask.
//

dst[0] = cmp_neq_f32(src[0], src[1]);
