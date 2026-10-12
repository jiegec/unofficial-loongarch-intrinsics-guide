// v_cmp_eq_f32 -- Compares two f32 values lane by lane for equality and writes the lane mask to `RD8`.
//

dst[0] = cmp_eq_f32(src[0], src[1], policy);
