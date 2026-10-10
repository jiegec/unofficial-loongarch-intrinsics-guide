// v_cmp_nlg_f64 -- Compares two f64 values with the neither-less-nor-greater predicate, writing a lane mask.
//

dst[0] = cmp_nlg_f64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
