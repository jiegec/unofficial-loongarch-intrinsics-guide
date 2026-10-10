// v_cmp_gt_i32 -- Compares two 32-bit signed integers per lane with greater-than, leaving the lane mask in the scalar destination `RD8`.
//

dst[0] = cmp_gt_i32(src[0], src[1]);
