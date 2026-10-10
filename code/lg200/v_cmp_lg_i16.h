// v_cmp_lg_i16 -- Compares two signed 16-bit integers lane by lane with the ordered-inequality predicate and writes one lane mask.
//

dst[0] = cmp_neq_i16(src[0], src[1]);
