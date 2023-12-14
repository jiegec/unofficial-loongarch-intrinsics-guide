for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == imm) ? b.dword[0] : a.dword[i];
}
