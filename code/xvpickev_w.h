for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? b.word[i * 2] : a.word[(i - 2) * 2];
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? b.word[(i - 2) * 2] : a.word[(i - 4) * 2];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[0];
  dst.word[1] = b.word[2];
  dst.word[2] = a.word[0];
  dst.word[3] = a.word[2];
  dst.word[4] = b.word[4];
  dst.word[5] = b.word[6];
  dst.word[6] = a.word[4];
  dst.word[7] = a.word[6];
}
