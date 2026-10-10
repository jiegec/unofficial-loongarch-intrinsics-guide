// v_cmp_eq_i16 -- Compares two signed 16-bit integers lane by lane for equality and writes the lane mask to `RD8`.
//

dst[0] = cmp_eq_i16(src[0], src[1]);
