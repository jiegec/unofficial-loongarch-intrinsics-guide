if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r = lhs & rhs;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
