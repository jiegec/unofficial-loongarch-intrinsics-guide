for (int i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i - 1] : b.half[i];
}
