// v_cmp_imm_eq_u16 -- Compares each u16 lane against the instruction literal for equality; writes a lane mask.
//

dst[0] = cmp_eq_u16(src[0], src[1]);
