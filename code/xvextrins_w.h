int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i == ((imm >> 4) & 3)) ? b.word[imm & 3] : a.word[i];
}
for (; i < 8; i++) {
  dst.word[i] = (i - 4 == ((imm >> 4) & 3)) ? b.word[(imm & 3) + 4] : a.word[i];
}
