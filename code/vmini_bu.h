for (int i = 0; i < 16; i++) {
  dst.byte[i] = min((u8)a.byte[i], (u8)imm);
}
