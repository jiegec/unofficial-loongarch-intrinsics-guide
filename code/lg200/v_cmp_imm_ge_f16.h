// v_cmp_imm_ge_f16 -- Compares each f16 lane against the instruction literal for greater-or-equal; writes a lane mask.
//

dst[0] = cmp_ge_f16(src[0], src[1]);
