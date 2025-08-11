for (int i = 0; i < 16; i++) {
  if ((a.half[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.half[i] = 0;
  } else if ((a.half[i] % 16) < 8) {
    dst.half[i] = c.half[(a.half[i] % 16) + ((i >= 8) ? 8 : 0)];
  } else {
    dst.half[i] = b.half[(a.half[i] % 16) + ((i >= 8) ? 0 : -8)];
  }
}
