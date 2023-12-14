for (int i = 0; i < 16; i++) {
  if ((a.half[i] % 32) < 16) {
    dst.half[i] = c.half[a.half[i] % 16];
  } else {
    dst.half[i] = b.half[a.half[i] % 16];
  }
}