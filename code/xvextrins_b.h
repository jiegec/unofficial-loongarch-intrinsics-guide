int i;
for (i = 0; i < 16; i++) {
  dst.byte[i] = (i == ((imm >> 4) & 15)) ? b.byte[imm & 15] : a.byte[i];
}
for (; i < 32; i++) {
  dst.byte[i] =
      (i - 16 == ((imm >> 4) & 15)) ? b.byte[(imm & 15) + 16] : a.byte[i];
}
