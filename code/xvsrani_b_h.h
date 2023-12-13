for (int i = 0; i < 32; i++) {
  dst.byte[i] =
      (i < 16) ? (s8)((s16)b.half[i] >> imm) : (s8)((s16)a.half[i - 16] >> imm);
}
