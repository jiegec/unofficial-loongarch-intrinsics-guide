for (int i = 0; i < 8; i++) {
  if (i < 4) {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)b.word[i];
    } else {
      dst.half[i] = (s16)(((s32)b.word[i] >> imm) +
                          (((s32)b.word[i] >> (imm - 1)) & 0x1));
    }
  } else {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)a.word[i - 4];
    } else {
      dst.half[i] = (s16)(((s32)a.word[i - 4] >> imm) +
                          (((s32)a.word[i - 4] >> (imm - 1)) & 0x1));
    }
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    if (imm == 0) {
      dst.half[i] = (s16)(s32)b.word[i - 4];
    } else {
      dst.half[i] = (s16)(((s32)b.word[i - 4] >> imm) +
                          (((s32)b.word[i - 4] >> (imm - 1)) & 0x1));
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
