if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t r = ((uint32_t)a >> 1) | (ARMFLAGS.C ? 0x80000000 : 0);
  ARMFLAGS.C = a & 1;
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
