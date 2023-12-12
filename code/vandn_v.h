for (int i = 0; i < 2; i++) {
  dst.dword[i] = b.dword[i] & (~a.dword[i]);
}