for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == ((imm >> 4) & 7)) ? b.word[imm & 7] : a.word[i];
}
