for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i] : b.dword[i + 1];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[1];
  dst.dword[1] = a.dword[1];
}
