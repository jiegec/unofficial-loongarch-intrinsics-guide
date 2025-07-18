for (int i = 0; i < 4; i++) {
  dst.dword[i] = (i == 0) ? a.dword[imm] : 0;
}

// Expands to:

if (0) {
  dst.dword[0] = a.dword[imm];
  dst.dword[1] = 0;
  dst.dword[2] = 0;
  dst.dword[3] = 0;
}
