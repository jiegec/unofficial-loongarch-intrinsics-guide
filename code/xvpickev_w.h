for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2) ? b.word[i * 2] : a.word[(i - 2) * 2];
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6) ? b.word[(i - 2) * 2] : a.word[(i - 4) * 2];
}
