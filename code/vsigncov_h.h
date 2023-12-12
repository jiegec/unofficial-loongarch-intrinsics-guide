for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (a.half[i] == 0) ? 0 : ((s16)a.half[i] > 0 ? b.half[i] : -b.half[i]);
}
