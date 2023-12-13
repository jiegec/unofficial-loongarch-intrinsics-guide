for (int i = 0; i < 16; i++) {
  dst.half[i] = (i == ((imm >> 4) & 15)) ? b.half[imm & 15] : a.half[i];
}
