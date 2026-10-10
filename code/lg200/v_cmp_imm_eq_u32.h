// v_cmp_imm_eq_u32 -- Compares each u32 lane against the instruction literal for equality; writes a lane mask.
//

dst[0] = cmp_eq_u32(src[0], src[1]);
