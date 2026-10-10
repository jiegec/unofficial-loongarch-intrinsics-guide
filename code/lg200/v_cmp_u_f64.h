// v_cmp_u_f64 -- Compares two f64 values with the unordered predicate: `d = isnan(a) || isnan(b)`.
//

dst[0] = cmp_u_f64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
