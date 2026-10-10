// v_cmp_ge_f32 -- Compares two f32 values per lane with greater-or-equal, leaving the lane mask in the scalar destination `RD8`.
//

dst[0] = cmp_ge_f32(src[0], src[1]);
