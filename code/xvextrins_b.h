for (int i = 0; i < 32; i++) {
  dst.byte[i] = (i == ((imm >> 4) & 31)) ? b.byte[imm & 31] : a.byte[i];
}
