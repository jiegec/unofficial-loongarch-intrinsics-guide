// v_cmp_imm_lg_u16 -- Compares each 16-bit unsigned lane with the literal for ordered inequality; writes a lane mask.
//

dst[0] = cmp_neq_u16(src[0], src[1]);
