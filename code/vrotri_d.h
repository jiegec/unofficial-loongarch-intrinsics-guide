for (int i = 0; i < 2; i++) {
  dst.dword[i] = (a.dword[i] >> imm) | (a.dword[i] << (64 - imm));
}
