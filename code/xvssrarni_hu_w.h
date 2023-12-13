for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp;
    if (imm == 0) {
      temp = (s32)b.word[i];
    } else {
      temp = ((s32)b.word[i] >> imm) + (((s32)b.word[i] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    s32 temp;
    if (imm == 0) {
      temp = (s32)a.word[i - 4];
    } else {
      temp =
          ((s32)a.word[i - 4] >> imm) + (((s32)a.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp;
    if (imm == 0) {
      temp = (s32)b.word[i - 4];
    } else {
      temp =
          ((s32)b.word[i - 4] >> imm) + (((s32)b.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    s32 temp;
    if (imm == 0) {
      temp = (s32)a.word[i - 8];
    } else {
      temp =
          ((s32)a.word[i - 8] >> imm) + (((s32)a.word[i - 8] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  }
}
