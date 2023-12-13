for (int i = 0; i < 16; i++) {
  dst.half[i] =
      (i < 8) ? (u16)((u32)b.word[i] >> imm) : (u16)((u32)a.word[i - 8] >> imm);
}
