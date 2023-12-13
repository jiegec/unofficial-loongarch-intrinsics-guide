for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == ((imm >> 4) & 3)) ? b.dword[imm & 3] : a.dword[i];
}
