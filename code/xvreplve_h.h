for (int i = 0; i < 8; i++) {
  dst.half[i] = a.half[idx % 8];
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = a.half[(idx % 8) + 8];
}
