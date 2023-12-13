for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp;
    if (imm == 0) {
      temp = (u32)b.word[i];
    } else {
      temp = ((u32)b.word[i] >> imm) + (((u32)b.word[i] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp;
    if (imm == 0) {
      temp = (u32)a.word[i - 4];
    } else {
      temp =
          ((u32)a.word[i - 4] >> imm) + (((u32)a.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp;
    if (imm == 0) {
      temp = (u32)b.word[i - 4];
    } else {
      temp =
          ((u32)b.word[i - 4] >> imm) + (((u32)b.word[i - 4] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp;
    if (imm == 0) {
      temp = (u32)a.word[i - 8];
    } else {
      temp =
          ((u32)a.word[i - 8] >> imm) + (((u32)a.word[i - 8] >> (imm - 1)) & 1);
    }
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  }
}
