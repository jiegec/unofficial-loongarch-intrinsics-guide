for (int i = 0; i < 4; i++) {
  if ((a.dword[i] % 256) >= 64 && MACHINE_3C5000) {
    // Caveat: observed in 3C5000 (LA464)
    dst.dword[i] = 0;
  } else if ((a.dword[i] % 4) < 2) {
    dst.dword[i] = c.dword[(a.dword[i] % 4) + ((i >= 2) ? 2 : 0)];
  } else {
    dst.dword[i] = b.dword[(a.dword[i] % 4) + ((i >= 2) ? 0 : -2)];
  }
}
