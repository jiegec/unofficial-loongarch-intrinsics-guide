for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == 0) ? a.dword[imm] : 0;
}
