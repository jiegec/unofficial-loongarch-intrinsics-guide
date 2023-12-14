for (int i = 0; i < 16; i++) {
  if ((c.half[i] % 256) >= 64) {
    dst.half[i] = 0;
  } else if ((c.half[i] % 16) < 8) {
    dst.half[i] = b.half[(c.half[i] % 16) + ((i >= 8) ? 8 : 0)];
  } else {
    dst.half[i] = a.half[(c.half[i] % 16) + ((i >= 8) ? 0 : -8)];
  }
}
