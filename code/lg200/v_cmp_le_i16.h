// v_cmp_le_i16 -- Compares two signed 16-bit integers lane by lane with the less-than-or-equal predicate and writes one lane mask.
//

dst[0] = cmp_le_i16(src[0], src[1]);
