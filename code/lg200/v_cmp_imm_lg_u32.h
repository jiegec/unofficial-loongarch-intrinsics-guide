// v_cmp_imm_lg_u32 -- Compares each 32-bit unsigned lane with the literal for ordered inequality; writes a lane mask.
//

dst[0] = cmp_neq_u32(src[0], src[1]);
