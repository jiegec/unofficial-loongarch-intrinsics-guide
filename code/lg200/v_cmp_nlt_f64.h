// v_cmp_nlt_f64 -- Compares two f64 elements for the negated less-than condition, writing a lane mask.
//

dst[0] = cmp_nlt_f64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
