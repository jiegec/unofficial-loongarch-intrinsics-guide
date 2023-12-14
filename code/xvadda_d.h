for (int i = 0; i < 4; i++) {
  dst.dword[i] = abs((s64)a.dword[i]) + abs((s64)b.dword[i]);
}
