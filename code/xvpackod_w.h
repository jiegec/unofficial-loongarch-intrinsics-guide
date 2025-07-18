for (int i = 0; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i] : b.word[i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[1];
  dst.word[1] = a.word[1];
  dst.word[2] = b.word[3];
  dst.word[3] = a.word[3];
  dst.word[4] = b.word[5];
  dst.word[5] = a.word[5];
  dst.word[6] = b.word[7];
  dst.word[7] = a.word[7];
}
