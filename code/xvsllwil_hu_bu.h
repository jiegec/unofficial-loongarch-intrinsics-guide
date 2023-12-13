for (int i = 0; i < 8; i++) {
  dst.half[i] = (u16)(u8)a.byte[i] << imm;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (u16)(u8)a.byte[i + 8] << imm;
}
