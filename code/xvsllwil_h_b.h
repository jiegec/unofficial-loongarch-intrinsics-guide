for (int i = 0; i < 16; i++) {
  dst.half[i] = (s16)(s8)a.byte[i] << imm;
}
