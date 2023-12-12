for (int i = 0; i < 4; i++) {
  dst.word[i] = (i == ((imm >> 4) & 3)) ? b.word[imm & 3] : a.word[i];
}
