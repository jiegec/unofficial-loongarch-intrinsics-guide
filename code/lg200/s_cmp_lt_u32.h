// s_cmp_lt_u32 -- Compares the two unsigned 32-bit integer operands with
// the less-than predicate, and writes 0xffff for true or zero for false to
// SCC or the scalar destination.
// The comparison does not modify either source; only the value result is
// modeled here.
dst[0] = cmp_lt_u32(src[0], src[1]);
