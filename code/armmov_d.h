if (arm_cond_holds(ARMFLAGS, cond)) {
  uint64_t r = a;
  ARMFLAGS.N = (r >> 63) & 1;
  ARMFLAGS.Z = r == 0;
}
