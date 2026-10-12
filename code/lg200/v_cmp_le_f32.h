// v_cmp_le_f32 -- Compares two f32 values lane by lane with the less-than-or-equal predicate and writes one lane mask.
//

dst[0] = cmp_le_f32(src[0], src[1], policy);
