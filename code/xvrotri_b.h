for (int i = 0; i < 32; i++) {
  dst.byte[i] = (a.byte[i] >> imm) | (a.byte[i] << (8 - imm));
}
