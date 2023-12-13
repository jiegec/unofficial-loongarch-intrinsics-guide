for (int i = 0; i < 32; i++) {
  dst.byte[i] =
      (i < 16) ? (u8)((u16)b.half[i] >> imm) : (u8)((u16)a.half[i - 16] >> imm);
}
