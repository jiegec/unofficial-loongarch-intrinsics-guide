for (int i = 0; i < 8; i++) {
  dst.word[i] =
      (s32)(s16)b.half[2 * i] * (s32)(s16)c.half[2 * i] + (s32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((s32)((s16)b.half[0])) * ((s32)((s16)c.half[0]))) + ((s32)a.word[0]);
  dst.word[1] =
      (((s32)((s16)b.half[2])) * ((s32)((s16)c.half[2]))) + ((s32)a.word[1]);
  dst.word[2] =
      (((s32)((s16)b.half[4])) * ((s32)((s16)c.half[4]))) + ((s32)a.word[2]);
  dst.word[3] =
      (((s32)((s16)b.half[6])) * ((s32)((s16)c.half[6]))) + ((s32)a.word[3]);
  dst.word[4] =
      (((s32)((s16)b.half[8])) * ((s32)((s16)c.half[8]))) + ((s32)a.word[4]);
  dst.word[5] =
      (((s32)((s16)b.half[10])) * ((s32)((s16)c.half[10]))) + ((s32)a.word[5]);
  dst.word[6] =
      (((s32)((s16)b.half[12])) * ((s32)((s16)c.half[12]))) + ((s32)a.word[6]);
  dst.word[7] =
      (((s32)((s16)b.half[14])) * ((s32)((s16)c.half[14]))) + ((s32)a.word[7]);
}
