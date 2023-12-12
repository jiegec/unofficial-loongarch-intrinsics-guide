for (int i = 0; i < 4; i++) {
  if ((a.word[i] % 256) >= 64) {
    // Caveat: observed in 3C5000, but not in QEMU
    dst.word[i] = 0;
  } else if ((a.word[i] % 8) < 4) {
    dst.word[i] = c.word[a.word[i] % 4];
  } else {
    dst.word[i] = b.word[a.word[i] % 4];
  }
}