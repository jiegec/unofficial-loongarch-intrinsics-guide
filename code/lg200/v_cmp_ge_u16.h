// v_cmp_ge_u16 -- Compares two 16-bit unsigned integers per lane with greater-or-equal, leaving the lane mask in the scalar destination `RD8`.
//

dst[0] = cmp_ge_u16(src[0], src[1]);
