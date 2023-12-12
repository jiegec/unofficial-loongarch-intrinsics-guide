for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i == ((imm >> 4) & 15)) ? b.byte[imm & 15] : a.byte[i];
}
