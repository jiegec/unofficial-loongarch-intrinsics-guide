for (int i = 0; i < 4; i++) {
  dst.word[i] = (s32)(s16)a.half[i] << imm;
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (s32)(s16)a.half[i + 4] << imm;
}

// Expands to:

if (0) {
  dst.word[0] = ((s32)((s16)a.half[0])) << imm;
  dst.word[1] = ((s32)((s16)a.half[1])) << imm;
  dst.word[2] = ((s32)((s16)a.half[2])) << imm;
  dst.word[3] = ((s32)((s16)a.half[3])) << imm;
  dst.word[4] = ((s32)((s16)a.half[8])) << imm;
  dst.word[5] = ((s32)((s16)a.half[9])) << imm;
  dst.word[6] = ((s32)((s16)a.half[10])) << imm;
  dst.word[7] = ((s32)((s16)a.half[11])) << imm;
}
