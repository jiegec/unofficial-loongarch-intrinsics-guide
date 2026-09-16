// s_cmp_le_i32 -- Compares the two signed 32-bit integer operands with
// the less-than-or-equal predicate, and writes 0xffff for true or zero for
// false to SCC or the scalar destination.
// The comparison does not modify either source; only the value result is
// modeled here.
dst[0] = cmp_le_i32(src[0], src[1]);
