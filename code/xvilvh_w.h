int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 2] : b.word[i / 2 + 2];
}
for (; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 4] : b.word[i / 2 + 4];
}

// Expands to:

if (0) {
  dst.word[0] = b.word[2];
  dst.word[1] = a.word[2];
  dst.word[2] = b.word[3];
  dst.word[3] = a.word[3];
  dst.word[4] = b.word[6];
  dst.word[5] = a.word[6];
  dst.word[6] = b.word[7];
  dst.word[7] = a.word[7];
}
