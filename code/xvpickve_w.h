for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == 0) ? a.word[imm] : 0;
}
