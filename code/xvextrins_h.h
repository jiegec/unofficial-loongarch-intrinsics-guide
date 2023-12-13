int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (i == ((imm >> 4) & 7)) ? b.half[imm & 7] : a.half[i];
}
for (; i < 16; i++) {
  dst.half[i] = (i - 8 == ((imm >> 4) & 7)) ? b.half[(imm & 7) + 8] : a.half[i];
}
