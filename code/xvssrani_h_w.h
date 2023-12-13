for (int i = 0; i < 16; i++) {
  if (i < 8) {
    s32 temp = (s32)b.word[i] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    s32 temp = (s32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  }
}
