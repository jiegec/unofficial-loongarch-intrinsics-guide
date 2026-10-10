// v_cmp_imm_eq_f32 -- Compares each f32 lane against the instruction literal for equality; writes a lane mask.
//

dst[0] = cmp_eq_f32(src[0], src[1]);
