// v_cmp_imm_nlg_f32 -- Compares each f32 lane with the literal under the neither-less-nor-greater predicate, an unordered pair reporting true: `d = ((a == b) || unordered) ? 0xffff : 0x0000`.
//

dst[0] = cmp_nlg_f32(src[0], src[1]);
