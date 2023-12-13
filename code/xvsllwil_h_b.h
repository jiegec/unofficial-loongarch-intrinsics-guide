for (int i = 0; i < 8; i++) {
  dst.half[i] = (s16)(s8)a.byte[i] << imm;
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[i + 8] << imm;
}
