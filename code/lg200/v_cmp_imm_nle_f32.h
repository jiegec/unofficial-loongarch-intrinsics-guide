// v_cmp_imm_nle_f32 -- Compares each f32 lane with the literal under the not-less-or-equal predicate, an unordered pair reporting true: `d = ((a > b) || unordered) ? 0xffff : 0x0000`.
//

dst[0] = cmp_nle_f32_std(src[0], src[1]);
