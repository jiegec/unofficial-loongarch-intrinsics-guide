// v_cmp_imm_lt_u16 -- Compares the two unsigned 16-bit integer operands using the encoded immediate as the second operand, with the less-than predicate, and writes a per-lane mask (0xffff for true, zero for false) to the scalar destination. The comparison does not modify either source.
// The comparison does not modify either source; only the per-lane mask value is modeled here (the encoded immediate is the second operand).
dst[0] = cmp_lt_u16(src[0], src[1]);
