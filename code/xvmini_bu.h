for (int i = 0; i < 32; i++) {
  dst.byte[i] = min((u8)a.byte[i], (u8)imm);
}
