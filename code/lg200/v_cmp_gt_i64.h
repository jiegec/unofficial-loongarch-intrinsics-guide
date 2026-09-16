// v_cmp_gt_i64 -- Compares the two signed 64-bit integer operands with the greater-than predicate, and writes a per-lane mask (0xffff for true, zero for false) to the scalar destination
// The comparison does not modify either source; only the per-lane mask value (0xffff / 0) is modeled here.
dst[0] = cmp_gt_i64(((u64)src[0] | ((u64)src[0+1] << 32)), ((u64)src[2] | ((u64)src[2+1] << 32)), policy);
