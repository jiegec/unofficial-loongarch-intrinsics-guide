// v_cmp_lt_f16 -- Compares two f16 values lane by lane with the less-than predicate and writes one lane mask.
//

dst[0] = cmp_lt_f16(src[0], src[1], policy);
