for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i == imm) ? b : a.dword[i];
}
