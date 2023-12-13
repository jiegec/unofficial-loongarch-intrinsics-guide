for (int i = 0; i < 32; i++) {
  dst.byte[i] = (((s16)(s8)a.byte[i] * (s16)(s8)b.byte[i])) >> 8;
}
