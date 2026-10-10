// v_cmp_nle_f64 -- Compares two f64 values with the not-less-or-equal predicate, writing a lane mask.
//

dst[0] = cmp_nle_f64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
