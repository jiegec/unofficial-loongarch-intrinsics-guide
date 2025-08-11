for (int i = 0; i < 16; i++) {
  if ((a.half[i] % 256) >= 64 && MACHINE_3C5000) {
    // Caveat: observed in 3C5000 (LA464)
    dst.half[i] = 0;
  } else if ((a.half[i] % 16) < 8) {
    dst.half[i] = c.half[(a.half[i] % 16) + ((i >= 8) ? 8 : 0)];
  } else {
    dst.half[i] = b.half[(a.half[i] % 16) + ((i >= 8) ? 0 : -8)];
  }
}
