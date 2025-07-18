for (int i = 0; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] - (s32)(s16)b.half[2 * i + 1];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) - ((s32)((s16)b.half[1]));
  dst.word[1] = ((s32)((s16)a.half[3])) - ((s32)((s16)b.half[3]));
  dst.word[2] = ((s32)((s16)a.half[5])) - ((s32)((s16)b.half[5]));
  dst.word[3] = ((s32)((s16)a.half[7])) - ((s32)((s16)b.half[7]));
  dst.word[4] = ((s32)((s16)a.half[9])) - ((s32)((s16)b.half[9]));
  dst.word[5] = ((s32)((s16)a.half[11])) - ((s32)((s16)b.half[11]));
  dst.word[6] = ((s32)((s16)a.half[13])) - ((s32)((s16)b.half[13]));
  dst.word[7] = ((s32)((s16)a.half[15])) - ((s32)((s16)b.half[15]));
}
