for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == imm) ? b.word[0] : a.word[i];
}
