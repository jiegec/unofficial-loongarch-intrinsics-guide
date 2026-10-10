// v_cmp_le_u32 -- Compares two unsigned 32-bit integers lane by lane with the less-than-or-equal predicate and writes one lane mask.
//

dst[0] = cmp_le_u32(src[0], src[1]);
