// s_cmp -- Compares the two 32-bit scalar operands for equality and writes
// 0xffff for true or zero for false to the scalar destination.
// [HW-verified] equality materializes 0xffff / 0; the comparison does not
// modify either source.
dst[0] = cmp_eq_u32(src[0], src[1]);
