// v_cmp_gt_f16 -- Compares two f16 values per lane with greater-than, leaving the lane mask in the scalar destination `RD8`.
//

dst[0] = cmp_gt_f16(src[0], src[1]);
