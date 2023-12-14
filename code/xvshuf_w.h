for (int i = 0; i < 8; i++) {
  if ((a.word[i] % 16) < 8) {
    dst.word[i] = c.word[a.word[i] % 8];
  } else {
    dst.word[i] = b.word[a.word[i] % 8];
  }
}