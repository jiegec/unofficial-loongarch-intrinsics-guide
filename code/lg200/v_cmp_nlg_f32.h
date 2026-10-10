// v_cmp_nlg_f32 -- Compares two f32 values with the neither-less-nor-greater predicate, an unordered pair reporting true: `d = ((a == b) || unordered) ? 0xffff : 0x0000`.
//

dst[0] = cmp_nlg_f32(src[0], src[1]);
