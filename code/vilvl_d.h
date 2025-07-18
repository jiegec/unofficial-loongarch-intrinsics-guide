for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i / 2] : b.dword[i / 2];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[0];
  dst.dword[1] = a.dword[0];
}
