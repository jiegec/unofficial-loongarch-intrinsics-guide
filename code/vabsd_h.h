for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] > (s16)b.half[i]) ? (a.half[i] - b.half[i])
                                                  : (b.half[i] - a.half[i]);
}
