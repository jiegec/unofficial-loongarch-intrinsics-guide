for (int i = 0; i < 4; i++) {
  if ((a.word[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.word[i] = 0;
  } else if ((a.word[i] % 8) < 4) {
    dst.word[i] = c.word[a.word[i] % 4];
  } else {
    dst.word[i] = b.word[a.word[i] % 4];
  }
}
