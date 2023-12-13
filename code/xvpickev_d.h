for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i < 2) ? b.dword[i * 2] : a.dword[(i - 2) * 2];
}
