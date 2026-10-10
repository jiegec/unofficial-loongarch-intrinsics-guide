// v_cmp_imm_nle_f16 -- Compares each f16 lane with the literal under the not-less-or-equal predicate, an unordered pair reporting true: `d = ((a > b) || unordered) ? 0xffff : 0x0000`.
//

dst[0] = cmp_nle_f16(src[0], src[1]);
