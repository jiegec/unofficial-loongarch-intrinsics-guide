int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 4];
}
for (; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 8];
}

// Expands to:

if (0) {
  dst.word[0] = (s32)((s16)a.half[4]);
  dst.word[1] = (s32)((s16)a.half[5]);
  dst.word[2] = (s32)((s16)a.half[6]);
  dst.word[3] = (s32)((s16)a.half[7]);
  dst.word[4] = (s32)((s16)a.half[12]);
  dst.word[5] = (s32)((s16)a.half[13]);
  dst.word[6] = (s32)((s16)a.half[14]);
  dst.word[7] = (s32)((s16)a.half[15]);
}
