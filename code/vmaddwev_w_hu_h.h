for (int i = 0; i < 4; i++) {
  dst.word[i] =
      (u32)(u16)b.half[2 * i] * (s32)(s16)c.half[2 * i] + (s32)a.word[i];
}

// Expands to:

if (0) {
  dst.word[0] =
      (((u32)((u16)b.half[0])) * ((s32)((s16)c.half[0]))) + ((s32)a.word[0]);
  dst.word[1] =
      (((u32)((u16)b.half[2])) * ((s32)((s16)c.half[2]))) + ((s32)a.word[1]);
  dst.word[2] =
      (((u32)((u16)b.half[4])) * ((s32)((s16)c.half[4]))) + ((s32)a.word[2]);
  dst.word[3] =
      (((u32)((u16)b.half[6])) * ((s32)((s16)c.half[6]))) + ((s32)a.word[3]);
}
