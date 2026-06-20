if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t rhs = (uint32_t)b;
  uint32_t r;
  uint32_t c = rhs & 0xff;
  if (c == 0) {
    r = lhs;
  } else if (c < 32) {
    r = lhs >> c;
    ARMFLAGS.C = (lhs >> (c - 1)) & 1;
  } else if (c == 32) {
    r = 0;
    ARMFLAGS.C = (lhs >> 31) & 1;
  } else {
    r = 0;
    ARMFLAGS.C = 0;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
