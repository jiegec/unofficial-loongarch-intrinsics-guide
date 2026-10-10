// v_cmp_le_u16 -- Compares two unsigned 16-bit integers lane by lane with the less-than-or-equal predicate and writes one lane mask.
//

dst[0] = cmp_le_u16(src[0], src[1]);
