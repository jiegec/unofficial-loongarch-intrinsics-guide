for (int i = 0; i < 16; i++) {
  dst.byte[i] =
      (i < 8) ? (u8)((u16)b.half[i] >> imm) : (u8)((u16)a.half[i - 8] >> imm);
}
for (int i = 16; i < 32; i++) {
  dst.byte[i] = (i < 24) ? (u8)((u16)b.half[i - 8] >> imm)
                         : (u8)((u16)a.half[i - 16] >> imm);
}
