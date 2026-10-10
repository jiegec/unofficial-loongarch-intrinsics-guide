// v_cmp_eq_u32 -- Compares two unsigned 32-bit integers lane by lane for equality and writes the lane mask to `RD8`.
//

dst[0] = cmp_eq_u32(src[0], src[1]);
