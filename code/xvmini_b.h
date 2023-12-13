for (int i = 0; i < 32; i++) {
  dst.byte[i] = min((s8)a.byte[i], (s8)imm);
}
