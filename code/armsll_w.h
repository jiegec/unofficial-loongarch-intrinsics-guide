if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t lhs = (uint32_t)a;
  uint32_t c = (uint32_t)b & 0xff;
  uint32_t r;

  if (c == 0) {
    // C preserved, only N/Z updated from unshifted value.
    r = lhs;
  } else if (c < 32) {
    ARMFLAGS.C = (lhs >> (32 - c)) & 1;
    r = lhs << c;
  } else if (c == 32) {
    ARMFLAGS.C = lhs & 1;
    r = 0;
  } else {
    ARMFLAGS.C = 0;
    r = 0;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
