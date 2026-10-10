// v_cmp_eq_u64 -- Compares two unsigned 64-bit integers lane by lane for equality and writes the lane mask to `RD8`.
//

dst[0] = cmp_eq_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
