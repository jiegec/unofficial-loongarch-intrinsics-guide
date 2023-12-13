for (int i = 0; i < 16; i++) {
  dst.half[i] = ((s16)a.half[i] < imm) ? 0xFFFF : 0;
}
