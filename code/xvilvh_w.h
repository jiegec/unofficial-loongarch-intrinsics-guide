int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 2] : b.word[i / 2 + 2];
}
for (; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 4] : b.word[i / 2 + 4];
}
