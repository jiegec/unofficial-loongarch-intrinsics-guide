for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.dword[idx % 2];
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = a.dword[(idx % 2) + 2];
}
