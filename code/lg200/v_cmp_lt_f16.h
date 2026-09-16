// v_cmp_lt_f16 -- Compares the two 16-bit floating-point operands with the less-than predicate, and writes a per-lane mask (0xffff for true, zero for false) to the scalar destination
// The comparison does not modify either source; only the per-lane mask value (0xffff / 0) is modeled here.
dst[0] = cmp_lt_f16(src[0], src[1]);
