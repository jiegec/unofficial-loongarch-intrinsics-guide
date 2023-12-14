for (int i = 0; i < 16; i++) {
  dst.half[i] = abs((s16)a.half[i]) + abs((s16)b.half[i]);
}
