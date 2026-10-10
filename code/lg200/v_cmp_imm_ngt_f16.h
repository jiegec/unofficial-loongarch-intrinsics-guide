// v_cmp_imm_ngt_f16 -- Compares each f16 lane with the literal under the not-greater-than predicate, an unordered pair reporting true: `d = ((a <= b) || unordered) ? 0xffff : 0x0000`.
//

dst[0] = cmp_ngt_f16(src[0], src[1]);
