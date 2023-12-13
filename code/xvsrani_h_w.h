for (int i = 0; i < 16; i++) {
  dst.half[i] =
      (i < 8) ? (s16)((s32)b.word[i] >> imm) : (s16)((s32)a.word[i - 8] >> imm);
}
