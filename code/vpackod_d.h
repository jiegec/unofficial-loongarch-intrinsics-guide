for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i] : b.dword[i + 1];
}
