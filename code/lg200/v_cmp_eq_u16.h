// v_cmp_eq_u16 -- Compares two unsigned 16-bit integers lane by lane for equality and writes the lane mask to `RD8`.
//

dst[0] = cmp_eq_u16(src[0], src[1]);
