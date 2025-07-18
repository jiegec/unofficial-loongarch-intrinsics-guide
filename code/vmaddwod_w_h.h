for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)b.half[2 * i + 1] * (s32)(s16)c.half[2 * i + 1] +
                (s32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((s32)((s16)b.half[1])) * ((s32)((s16)c.half[1]))) + ((s32)a.word[0]);
  dst.word[1] =
      (((s32)((s16)b.half[3])) * ((s32)((s16)c.half[3]))) + ((s32)a.word[1]);
  dst.word[2] =
      (((s32)((s16)b.half[5])) * ((s32)((s16)c.half[5]))) + ((s32)a.word[2]);
  dst.word[3] =
      (((s32)((s16)b.half[7])) * ((s32)((s16)c.half[7]))) + ((s32)a.word[3]);
}
