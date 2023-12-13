for (int i = 0; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[2 * i] * (s16)(s8)b.byte[2 * i];
}
