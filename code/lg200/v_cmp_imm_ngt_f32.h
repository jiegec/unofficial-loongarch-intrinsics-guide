// v_cmp_imm_ngt_f32 -- Compares each f32 lane with the literal under the not-greater-than predicate, an unordered pair reporting true: `d = ((a <= b) || unordered) ? 0xffff : 0x0000`.
//

dst[0] = cmp_ngt_f32_std(src[0], src[1]);
