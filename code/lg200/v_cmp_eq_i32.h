// v_cmp_eq_i32 -- Compares two signed 32-bit integers lane by lane for equality and writes the lane mask to `RD8`.
//

dst[0] = cmp_eq_i32(src[0], src[1]);
