for (int i = 0; i < 16; i++) {
  if (c.byte[i] >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.byte[i] = 0;
  } else if ((c.byte[i] % 32) < 16) {
    dst.byte[i] = b.byte[c.byte[i] % 16];
  } else {
    dst.byte[i] = a.byte[c.byte[i] % 16];
  }
}
