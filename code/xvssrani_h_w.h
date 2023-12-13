for (int i = 0; i < 8; i++) {
  if (i < 4) {
    s32 temp = (s32)b.word[i] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    s32 temp = (s32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    s32 temp = (s32)b.word[i - 4] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  } else {
    s32 temp = (s32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<s32>(temp, -32768, 32767);
  }
}
