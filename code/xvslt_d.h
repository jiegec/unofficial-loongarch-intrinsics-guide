for (int i = 0; i < 4; i++) {
  dst.dword[i] = ((s64)a.dword[i] < (s64)b.dword[i]) ? 0xFFFFFFFFFFFFFFFF : 0;
}
