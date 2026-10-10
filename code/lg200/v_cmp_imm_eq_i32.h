// v_cmp_imm_eq_i32 -- Compares each i32 lane against the instruction literal for equality; writes a lane mask.
//

dst[0] = cmp_eq_i32(src[0], src[1]);
