for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[i] << imm;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 4] << imm;
}
