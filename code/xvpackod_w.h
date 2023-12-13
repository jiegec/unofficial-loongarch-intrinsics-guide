for (int i = 0; i < 8; i++) {
  dst.word[i] = (i % 2 == 1) ? a.word[i] : b.word[i + 1];
}
