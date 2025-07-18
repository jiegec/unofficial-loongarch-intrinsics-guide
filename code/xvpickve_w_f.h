for (int i = 0; i < 8; i++) {
  dst.word[i] = (i == 0) ? a.word[imm] : 0;
}

// Expands to:

if (0) {
  dst.word[0] = a.word[imm];
  dst.word[1] = 0;
  dst.word[2] = 0;
  dst.word[3] = 0;
  dst.word[4] = 0;
  dst.word[5] = 0;
  dst.word[6] = 0;
  dst.word[7] = 0;
}
