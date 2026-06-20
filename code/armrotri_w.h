if (arm_cond_holds(ARMFLAGS, cond)) {
  uint32_t v = (uint32_t)a;
  uint32_t r = imm == 0 ? v : (v >> imm) | (v << (32 - imm));
  if (imm != 0) {
    ARMFLAGS.C = (v >> (imm - 1)) & 1;
  }
  ARMFLAGS.N = (r >> 31) & 1;
  ARMFLAGS.Z = r == 0;
}
