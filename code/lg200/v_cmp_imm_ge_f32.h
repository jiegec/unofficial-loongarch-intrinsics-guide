// v_cmp_imm_ge_f32 -- Compares each f32 lane against the instruction literal for greater-or-equal; writes a lane mask.
//

dst[0] = cmp_ge_f32(src[0], src[1], policy);
