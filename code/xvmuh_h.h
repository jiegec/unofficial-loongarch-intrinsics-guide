for (int i = 0; i < 16; i++) {
  dst.half[i] = (((s32)(s16)a.half[i] * (s32)(s16)b.half[i])) >> 16;
}
