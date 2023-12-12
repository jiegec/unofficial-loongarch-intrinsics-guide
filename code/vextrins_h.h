for (int i = 0; i < 8; i++) {
  dst.half[i] = (i == ((imm >> 4) & 7)) ? b.half[imm & 7] : a.half[i];
}
