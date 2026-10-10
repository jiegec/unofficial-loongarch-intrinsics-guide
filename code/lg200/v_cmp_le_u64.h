// v_cmp_le_u64 -- Compares two unsigned 64-bit integers lane by lane with the less-than-or-equal predicate and writes one lane mask.
//

dst[0] = cmp_le_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
