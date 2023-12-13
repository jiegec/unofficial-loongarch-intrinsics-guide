for (int i = 0; i < 16; i++) {
  dst.half[i] = -b.half[i] * c.half[i] + a.half[i];
}
