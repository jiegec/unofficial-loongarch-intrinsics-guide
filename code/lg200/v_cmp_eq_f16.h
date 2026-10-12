// v_cmp_eq_f16 -- Compares two f16 values lane by lane for equality and writes the lane mask to `RD8`.
//

dst[0] = cmp_eq_f16(src[0], src[1], policy);
