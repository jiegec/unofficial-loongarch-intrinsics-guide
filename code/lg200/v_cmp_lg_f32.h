// v_cmp_lg_f32 -- Compares two f32 values lane by lane with the ordered-inequality predicate and writes one lane mask.
//

dst[0] = cmp_lg_f32(src[0], src[1]);
