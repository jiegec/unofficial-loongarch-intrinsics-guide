// v_cmp_lg_i32 -- Compares two signed 32-bit integers lane by lane with the ordered-inequality predicate and writes one lane mask.
//

dst[0] = cmp_neq_i32(src[0], src[1]);
