// v_cmp_eq_u16 -- Compares the two unsigned 16-bit integer operands with the equality predicate, and writes a per-lane mask (0xffff for true, zero for false) to the scalar destination
// The comparison does not modify either source; only the per-lane mask value (0xffff / 0) is modeled here.
dst[0] = cmp_eq_u16(src[0], src[1]);
