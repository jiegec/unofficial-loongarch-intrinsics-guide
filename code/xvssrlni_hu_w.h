for (int i = 0; i < 16; i++) {
  if (i < 8) {
    u32 temp = (u32)b.word[i] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  } else {
    u32 temp = (u32)a.word[i - 8] >> imm;
    dst.half[i] = clamp<u32>(temp, 0, 65535);
  }
}
