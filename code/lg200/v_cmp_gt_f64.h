// v_cmp_gt_f64 -- Compares two f64 values per lane with greater-than, leaving the lane mask in the scalar destination `RD8`.
//

dst[0] = cmp_gt_f64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
