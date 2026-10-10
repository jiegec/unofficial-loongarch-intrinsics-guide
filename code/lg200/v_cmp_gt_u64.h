// v_cmp_gt_u64 -- Compares two 64-bit unsigned integers lane by lane and writes the greater-than lane mask to a scalar register.
//

dst[0] = cmp_gt_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
