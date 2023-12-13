for (int i = 0; i < 4; i++) {
  dst.dword[i] = min((u64)a.dword[i], (u64)imm);
}
