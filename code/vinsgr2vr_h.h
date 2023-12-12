for (int i = 0; i < 8; i++) {
  dst.half[i] = (i == imm) ? b : a.half[i];
}
