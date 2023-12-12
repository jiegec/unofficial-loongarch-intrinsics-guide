for (int i = 0; i < 8; i++) {
  dst.half[i] = (b.half[i] == 0) ? 0 : ((s16)a.half[i] / (s16)b.half[i]);
}
