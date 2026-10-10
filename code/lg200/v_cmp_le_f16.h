// v_cmp_le_f16 -- Compares two f16 values lane by lane with the less-than-or-equal predicate and writes one lane mask.
//

dst[0] = cmp_le_f16(src[0], src[1]);
