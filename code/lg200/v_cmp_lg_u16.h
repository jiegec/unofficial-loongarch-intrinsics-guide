// v_cmp_lg_u16 -- Compares two unsigned 16-bit integers lane by lane with the ordered-inequality predicate and writes one lane mask.
//

dst[0] = cmp_neq_u16(src[0], src[1]);
