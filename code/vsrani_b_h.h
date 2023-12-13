for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (i < 8) ? (s8)((s16)b.half[i] >> imm) : (s8)((s16)a.half[i - 8] >> imm);
}
