for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1) ? b.dword[i * 2 + 1] : a.dword[(i - 1) * 2 + 1];
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (i < 3) ? b.dword[(i - 1) * 2 + 1] : a.dword[(i - 2) * 2 + 1];
}
