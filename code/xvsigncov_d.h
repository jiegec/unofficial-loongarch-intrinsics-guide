for (int i = 0; i < 4; i++) {
  dst.dword[i] =
      (a.dword[i] == 0) ? 0 : ((s64)a.dword[i] > 0 ? b.dword[i] : -b.dword[i]);
}
