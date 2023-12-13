for (int i = 0; i < 4; i++) {
  dst.dword[i] = a.dword[i] | ((u64)1 << (b.dword[i] % 64));
}
