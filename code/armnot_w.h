if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = ~(uint32_t)a;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
