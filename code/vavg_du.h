for (int i = 0;i < 2;i++) {
  dst.dword[i] = ((u64)a.dword[i] >> 1) + ((u64)b.dword[i] >> 1) + (a.dword[i] & b.dword[i] & 1);
}
