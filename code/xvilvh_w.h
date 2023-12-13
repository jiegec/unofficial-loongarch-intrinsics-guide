for (int i = 0; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i / 2 + 4] : b.word[i / 2 + 2];
}
