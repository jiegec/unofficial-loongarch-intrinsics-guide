for (int i = 0; i < 32; i++) {
  if ((c.byte[i] % 256) >= 64 && MACHINE_3C5000) {
    // Caveat: observed in 3C5000
    dst.byte[i] = 0;
  } else if ((c.byte[i] % 32) < 16) {
    dst.byte[i] = b.byte[(c.byte[i] % 32) + ((i >= 16) ? 16 : 0)];
  } else {
    dst.byte[i] = a.byte[(c.byte[i] % 32) + ((i >= 16) ? 0 : -16)];
  }
}
