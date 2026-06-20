if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint64_t wide = (uint64_t)lhs + rhs;
  uint32_t r = (uint32_t)wide;
  ARMFLAGS.C = (wide >> 32) & 1;
  ARMFLAGS.V = ((~(lhs ^ rhs)) & (lhs ^ r) & 0x80000000) != 0;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
