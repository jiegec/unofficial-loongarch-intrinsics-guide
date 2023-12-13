for (int i = 0; i < 8; i++) {
  if (i < 4) {
    u32 temp = (u32)b.word[i] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp = (u32)a.word[i - 4] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  }
}
for (int i = 8; i < 16; i++) {
  if (i < 12) {
    u32 temp = (u32)b.word[i - 4] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  } else {
    u32 temp = (u32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 32767);
  }
}
