for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (i < 4) ? (s16)((s32)b.word[i] >> imm) : (s16)((s32)a.word[i - 4] >> imm);
}
