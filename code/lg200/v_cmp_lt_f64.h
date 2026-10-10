// v_cmp_lt_f64 -- Compares two f64 values for less-than and writes a lane mask: `d = (a < b) ? 0xffff : 0x0000`.
//

dst[0] = cmp_lt_f64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
