// v_cmp_gt_f32 -- Compares two f32 values per lane with greater-than, leaving the lane mask in the scalar destination `RD8`.
//

dst[0] = cmp_gt_f32(src[0], src[1]);
