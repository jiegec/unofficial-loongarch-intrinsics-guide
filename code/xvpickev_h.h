for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? b.half[i * 2] : a.half[(i - 4) * 2];
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (i < 12) ? b.half[(i - 4) * 2] : a.half[(i - 8) * 2];
}
