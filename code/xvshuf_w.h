for (int i = 0; i < 8; i++) {
  if ((a.word[i] % 256) >= 64 && MACHINE_3C5000) {
    dst.word[i] = 0;
  } else if ((a.word[i] % 8) < 4) {
    dst.word[i] = c.word[(a.word[i] % 8) + ((i >= 4) ? 4 : 0)];
  } else {
    dst.word[i] = b.word[(a.word[i] % 8) + ((i >= 4) ? 0 : -4)];
  }
}
