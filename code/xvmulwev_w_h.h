for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i] * (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) * ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[2])) * ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[4])) * ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[6])) * ((s32)((s16)b.half[6]));
  dst.word[4] = ((s32)((s16)a.half[8])) * ((s32)((s16)b.half[8]));
  dst.word[5] = ((s32)((s16)a.half[10])) * ((s32)((s16)b.half[10]));
  dst.word[6] = ((s32)((s16)a.half[12])) * ((s32)((s16)b.half[12]));
  dst.word[7] = ((s32)((s16)a.half[14])) * ((s32)((s16)b.half[14]));
}
