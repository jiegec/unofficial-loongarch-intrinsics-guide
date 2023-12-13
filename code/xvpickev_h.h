for (int i = 0; i < 16; i++) {
  dst.half[i] = (i < 8) ? b.half[i * 2] : a.half[(i - 4) * 2];
}
