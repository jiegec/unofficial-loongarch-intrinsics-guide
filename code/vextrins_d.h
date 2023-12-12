for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i == ((imm >> 4) & 1)) ? b.dword[imm & 1] : a.dword[i];
}
