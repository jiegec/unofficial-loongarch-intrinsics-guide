for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (i < 8) ? (s8)((s16)b.half[i] >> imm) : (s8)((s16)a.half[i - 8] >> imm);
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? (s8)((s16)b.half[i - 8] >> imm)
                         : (s8)((s16)a.half[i - 16] >> imm);
}
