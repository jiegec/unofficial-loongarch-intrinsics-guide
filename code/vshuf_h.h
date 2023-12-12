for (int i = 0; i < 8; i++) {
  if ((a.half[i] % 256) >= 64) {
    // Caveat: observed in 3C5000, but not in QEMU
    dst.half[i] = 0;
  } else if ((a.half[i] % 16) < 8) {
    dst.half[i] = c.half[a.half[i] % 8];
  } else {
    dst.half[i] = b.half[a.half[i] % 8];
  }
}