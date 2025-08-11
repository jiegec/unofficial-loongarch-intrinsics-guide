for (int i = 0; i < 8; i++) {
  if ((a.half[i] % 256) >= 64 && MACHINE_3C5000) {
    // Caveat: observed in 3C5000 (LA464)
    dst.half[i] = 0;
  } else if ((a.half[i] % 16) < 8) {
    dst.half[i] = c.half[a.half[i] % 8];
  } else {
    dst.half[i] = b.half[a.half[i] % 8];
  }
}
