for (int i = 0; i < 32; i++) {
  dst.byte[i] = (i < 16) ? b.byte[i * 2 + 1] : a.byte[(i - 8) * 2 + 1];
}
