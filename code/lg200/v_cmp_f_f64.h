// v_cmp_f_f64 -- Compares the two 64-bit floating-point operands with the always false predicate, and writes a per-lane mask (0xffff for true, zero for false) to the scalar destination
// The comparison does not modify either source; only the per-lane mask value (0xffff / 0) is modeled here.
dst[0] = 0u;
