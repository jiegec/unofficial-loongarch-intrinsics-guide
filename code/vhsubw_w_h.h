for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[2 * i + 1] - (s32)(s16)b.half[2 * i];
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[1])) - ((s32)((s16)b.half[0]));
  dst.word[1] = ((s32)((s16)a.half[3])) - ((s32)((s16)b.half[2]));
  dst.word[2] = ((s32)((s16)a.half[5])) - ((s32)((s16)b.half[4]));
  dst.word[3] = ((s32)((s16)a.half[7])) - ((s32)((s16)b.half[6]));
}
