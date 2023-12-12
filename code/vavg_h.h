for (int i = 0; i < 8; i++) {
  dst.half[i] = ((s16)a.half[i] >> 1) + ((s16)b.half[i] >> 1) + (a.half[i] & b.half[i] & 1);
}