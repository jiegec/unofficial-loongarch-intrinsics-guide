// v_cmp_nlg_f16 -- Compares two f16 values with the neither-less-nor-greater predicate, an unordered pair reporting true: `d = ((a == b) || unordered) ? 0xffff : 0x0000`.
//

dst[0] = cmp_nlg_f16(src[0], src[1]);
