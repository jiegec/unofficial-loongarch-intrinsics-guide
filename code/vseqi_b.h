for (int i = 0; i < 16; i++) {
  dst.byte[i] = ((s8)a.byte[i] == imm) ? 0xFF : 0;
}
