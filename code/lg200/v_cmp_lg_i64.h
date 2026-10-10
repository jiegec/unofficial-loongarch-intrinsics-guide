// v_cmp_lg_i64 -- Compares two signed 64-bit integers lane by lane with the ordered-inequality predicate and writes one lane mask.
//

dst[0] = cmp_neq_i64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
