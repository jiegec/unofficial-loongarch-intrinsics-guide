for (int i = 0; i < 4; i++) {
  if ((a.dword[i] % 8) < 4) {
    dst.dword[i] = c.dword[a.dword[i] % 4];
  } else {
    dst.dword[i] = b.dword[a.dword[i] % 4];
  }
}