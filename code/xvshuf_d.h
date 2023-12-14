for (int i = 0; i < 4; i++) {
  if ((a.dword[i] % 4) < 2) {
    dst.dword[i] = c.dword[(a.dword[i] % 4) + ((i >= 2) ? 2 : 0)];
  } else {
    dst.dword[i] = b.dword[(a.dword[i] % 4) + ((i >= 2) ? 0 : -2)];
  }
}
