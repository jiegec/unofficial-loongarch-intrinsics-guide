for (int i = 0; i < 8; i++) {
  dst.half[i] = (a.half[i] >> imm) | (a.half[i] << (16 - imm));
}
