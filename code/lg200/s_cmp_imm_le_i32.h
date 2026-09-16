// s_cmp_imm_le_i32 -- Compares the two signed 32-bit integer operands using
// the encoded immediate as the second operand, with the
// less-than-or-equal predicate, and writes 0xffff for true or zero for
// false to SCC or the scalar destination.
// True materializes 0xffff, false 0, written to SCC or the scalar
// destination; only the value result is modeled here (signed compare).
dst[0] = ((s32)src[0] <= (s32)src[1]) ? 0xffffu : 0u;
