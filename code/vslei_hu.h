for (int i = 0; i < 8; i++) {
  dst.half[i] = ((u16)a.half[i] <= imm) ? 0xFFFF : 0;
}
