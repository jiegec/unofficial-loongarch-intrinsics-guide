for (int i = 0; i < 4; i++) {
  dst.word[i] = (i == imm) ? b : a.word[i];
}
