if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t raw_count = rhs & 0xff;
  uint32_t c = rhs & 0x1f;
  unsigned n = c % 32;
  uint32_t r = n == 0 ? lhs : (lhs >> n) | (lhs << (32 - n));
  if (raw_count != 0) {
    ARMFLAGS.C = n == 0 ? ((lhs >> 31) & 1) : ((lhs >> (n - 1)) & 1);
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
