for (int i = 0; i < 16; i++) {
  dst.half[i] = (i == imm) ? b : a.half[i];
}
