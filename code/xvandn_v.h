for (int i = 0; i < 4; i++) {
  dst.dword[i] = b.dword[i] & (~a.dword[i]);
}