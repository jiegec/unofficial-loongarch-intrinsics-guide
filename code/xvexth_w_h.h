int i;
for (i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[4 + i];
}
for (; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[8 + i];
}
