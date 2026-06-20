if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t borrow_in = ARMFLAGS.C ? 0 : 1;
  uint64_t subtrahend = (uint64_t)rhs + borrow_in;
  uint32_t r = (uint32_t)(lhs - subtrahend);
  ARMFLAGS.C = lhs < subtrahend;
  ARMFLAGS.V = ((lhs ^ (uint32_t)subtrahend) & (lhs ^ r) & 0x80000000) != 0;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
