for (int i = 0; i < 4; i++) {
  dst.word[i] = (a.word[i] >> imm) | (a.word[i] << (32 - imm));
}
