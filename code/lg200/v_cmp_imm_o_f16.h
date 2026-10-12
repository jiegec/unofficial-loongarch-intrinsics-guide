// v_cmp_imm_o_f16 -- Ordered compare of f16 elements against the literal; writes a lane mask.
//

dst[0] = cmp_o_f16(src[0], src[1], policy);
