// v_cmp_imm_f_u16 -- Compares the two unsigned 16-bit integer operands using the encoded immediate as the second operand, with the always false predicate, and writes a per-lane mask (0xffff for true, zero for false) to the scalar destination. The comparison does not modify either source.
// The comparison does not modify either source; only the per-lane mask value is modeled here (the encoded immediate is the second operand).
dst[0] = 0u;
