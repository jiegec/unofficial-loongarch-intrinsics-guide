for (int i = 0; i < 16; i++) {
  if (c.byte[i] >= 64) {
    // Caveat: observed in 3C5000, but not in QEMU
    dst.byte[i] = 0;
  } else if ((c.byte[i] % 32) < 16) {
    dst.byte[i] = b.byte[c.byte[i] % 16];
  } else {
    dst.byte[i] = a.byte[c.byte[i] % 16];
  }
}