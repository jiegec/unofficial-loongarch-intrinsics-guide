for (int i = 0; i < 32; i++) {
  if (imm == 0) {
    dst.byte[i] = a.byte[i];
  } else {
    dst.byte[i] = ((s8)a.byte[i] >> imm) + (((s8)a.byte[i] >> (imm - 1)) & 0x1);
  }
}
