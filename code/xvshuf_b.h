for (int i = 0; i < 32; i++) {
  if ((c.byte[i] % 64) < 32) {
    dst.byte[i] = b.byte[c.byte[i] % 32];
  } else {
    dst.byte[i] = a.byte[c.byte[i] % 32];
  }
}