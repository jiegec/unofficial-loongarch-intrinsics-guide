// v_cmp_ge_u64 -- Compares two 64-bit unsigned integers per lane with greater-or-equal, leaving the lane mask in the scalar destination `RD8`.
//

dst[0] = cmp_ge_u64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
