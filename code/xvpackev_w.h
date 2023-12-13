for (int i = 0; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i - 1] : b.word[i];
}
