for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] - (s32)(s16)b.half[2 * i + 1];
}
