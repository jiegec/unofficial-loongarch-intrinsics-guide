for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i == imm) ? b : a.byte[i];
}
