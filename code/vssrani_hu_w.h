for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp = (s32)b.word[i] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  } else {
    s32 temp = (s32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<s32>(temp, 0, 65535);
  }
}
