for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[i] ^ ((u64)1 << imm);
}
