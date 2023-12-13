for (int i = 0; i < 4; i++) {
  dst.dword[i] = max((u64)a.dword[i], (u64)imm);
}
