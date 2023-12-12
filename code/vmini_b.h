for (int i = 0; i < 16; i++) {
  dst.byte[i] = min((s8)a.byte[i], (s8)imm);
}
