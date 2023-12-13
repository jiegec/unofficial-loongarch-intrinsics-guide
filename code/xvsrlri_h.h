for (int i = 0; i < 16; i++) {
  if (imm == 0) {
    dst.half[i] = a.half[i];
  } else {
    dst.half[i] = (a.half[i] >> imm) + ((a.half[i] >> (imm - 1)) & 0x1);
  }
}
