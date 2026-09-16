// s_cmp_imm_eq_i32 -- Compares the two signed 32-bit integer operands using the
// encoded immediate as the second operand, with the equality predicate, and
// writes 0xffff for true or zero for false to SCC or the scalar
// destination.
// True materializes 0xffff, false 0, written to SCC or the scalar
// destination; only the value result is modeled here.
dst[0] = cmp_eq_u32(src[0], src[1]);
