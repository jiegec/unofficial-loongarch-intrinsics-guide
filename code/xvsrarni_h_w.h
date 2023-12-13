for (int i = 0; i < 16; i++) {
  if (i < 8) {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)b.word[i];
    } else {
      dst.half[i] = (s16)(((s32)b.word[i] >> imm) +
                          (((s32)b.word[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)a.word[i - 8];
    } else {
      dst.half[i] = (s16)(((s32)a.word[i - 8] >> imm) +
                          (((s32)a.word[i - 8] >> (imm - 1)) & 0x1));
    }
  }
}
