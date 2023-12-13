for (int i = 0; i < 8; i++) {
  if (imm == 0) {
    dst.word[i] = a.word[i];
  } else {
    dst.word[i] = (a.word[i] >> imm) + ((a.word[i] >> (imm - 1)) & 0x1);
  }
}
