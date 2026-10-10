// v_cmp_tru_f64 -- Compares two f64 elements for the always-true condition, writing a lane mask.
//

dst[0] = cmp_tru(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
