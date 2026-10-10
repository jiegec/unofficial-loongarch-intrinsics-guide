// v_cmp_imm_nlt_f32 -- Compares each f32 lane with the literal for not-less-than, unordered included; writes a lane mask.
//

dst[0] = cmp_nlt_f32(src[0], src[1]);
