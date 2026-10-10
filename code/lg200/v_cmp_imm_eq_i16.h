// v_cmp_imm_eq_i16 -- Compares each i16 lane against the instruction literal for equality; writes a lane mask.
//

dst[0] = cmp_eq_i16(src[0], src[1]);
