// v_cmp_imm_ngt_f32 -- Compares the two 32-bit floating-point operands using the encoded immediate as the second operand, with the not-greater-than predicate, and writes a per-lane mask (0xffff for true, zero for false) to the scalar destination. The comparison does not modify either source.
// The comparison does not modify either source; only the per-lane mask value is modeled here (the encoded immediate is the second operand).
dst[0] = cmp_ngt_f32_std(src[0], src[1]);
