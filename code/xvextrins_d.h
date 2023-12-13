int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (i == ((imm >> 4) & 1)) ? b.dword[imm & 1] : a.dword[i];
}
for (; i < 4; i++) {
  dst.dword[i] =
      (i - 2 == ((imm >> 4) & 1)) ? b.dword[(imm & 1) + 2] : a.dword[i];
}
