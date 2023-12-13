for (int i = 0; i < 8; i++) {
  dst.half[i] =
      (i < 4) ? (u16)((u32)b.word[i] >> imm) : (u16)((u32)a.word[i - 4] >> imm);
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (i < 12) ? (u16)((u32)b.word[i - 4] >> imm)
                         : (u16)((u32)a.word[i - 8] >> imm);
}
