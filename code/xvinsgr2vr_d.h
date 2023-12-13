for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == imm) ? b : a.dword[i];
}
