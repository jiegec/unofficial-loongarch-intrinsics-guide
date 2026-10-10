// v_cmp_imm_eq_f16 -- Compares each f16 lane against the instruction literal for equality; writes a lane mask.
//

dst[0] = cmp_eq_f16(src[0], src[1]);
