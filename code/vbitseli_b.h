for (int i = 0; i < 16; i++) {
  dst.byte[i] = (~a.byte[i] & b.byte[i]) | (a.byte[i] & (u8)imm);
}