for (int i = 0; i < 16; i++) {
  dst.half[i] = (i < 8) ? b.half[i * 2 + 1] : a.half[(i - 8) * 2 + 1];
}
