for (int i = 0; i < 8; i++) {
  if (imm == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] =
        ((s32)a.word[i] >> imm) + (((s32)a.word[i] >> (imm - 1)) & 0x1);
  }
}
