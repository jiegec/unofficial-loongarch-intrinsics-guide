for (int i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 2] : b.word[i / 2 + 2];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[2];
  dst.word[1] = a.word[2];
  dst.word[2] = b.word[3];
  dst.word[3] = a.word[3];
}
