for (int i = 0; i < 2; i++) {
  if ((a.dword[i] % 256) >= 64 && (UARCH_LA264 || UARCH_LA464)) {
    // Caveat: observed in LA264 and LA464
    dst.dword[i] = 0;
  } else if ((a.dword[i] % 4) < 2) {
    dst.dword[i] = c.dword[a.dword[i] % 2];
  } else {
    dst.dword[i] = b.dword[a.dword[i] % 2];
  }
}
