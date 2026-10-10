// v_cmp_lg_u32 -- Compares two unsigned 32-bit integers lane by lane with the ordered-inequality predicate and writes one lane mask.
//

dst[0] = cmp_neq_u32(src[0], src[1]);
