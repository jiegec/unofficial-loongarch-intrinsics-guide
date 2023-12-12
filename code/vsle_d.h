for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] <= (s64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
}
