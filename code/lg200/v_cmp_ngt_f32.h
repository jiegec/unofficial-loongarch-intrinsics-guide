// v_cmp_ngt_f32 -- Compares the two 32-bit floating-point operands with the not-greater-than predicate, and writes a per-lane mask (0xffff for true, zero for false) to the scalar destination
// The comparison does not modify either source; only the per-lane mask value (0xffff / 0) is modeled here.
dst[0] = cmp_ngt_f32(src[0], src[1]);
