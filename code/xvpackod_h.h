for (int i = 0; i < 16; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i] : b.half[i + 1];
}
