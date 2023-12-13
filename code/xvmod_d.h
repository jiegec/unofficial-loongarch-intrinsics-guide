for (int i = 0; i < 4; i++) {
  dst.dword[i] = (b.dword[i] == 0) ? 0 : ((s64)a.dword[i] % (s64)b.dword[i]);
}
