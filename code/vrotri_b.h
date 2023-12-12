for (int i = 0; i < 16; i++) {
  dst.byte[i] = (a.byte[i] >> imm) | (a.byte[i] << (8 - imm));
}
