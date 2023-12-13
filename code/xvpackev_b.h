for (int i = 0; i < 32; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i - 1] : b.byte[i];
}
