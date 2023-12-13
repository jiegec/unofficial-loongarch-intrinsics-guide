for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == imm) ? b : a.word[i];
}
