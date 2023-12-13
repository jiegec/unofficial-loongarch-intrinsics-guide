for (int i = 0; i < 8; i++) {
  dst.word[i] =
      (s32)(s16)b.half[2 * i] * (s32)(s16)c.half[2 * i] + (s32)a.word[i];
}
