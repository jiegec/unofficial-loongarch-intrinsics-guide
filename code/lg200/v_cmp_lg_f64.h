// v_cmp_lg_f64 -- Compares two f64 values lane by lane with the ordered-inequality predicate and writes one lane mask.
//

dst[0] = cmp_lg_f64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
