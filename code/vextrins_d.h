for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i == ((imm >> 4) & 1)) ? b.dword[imm & 1] : a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] = (0 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[0]);
  dst.dword[1] = (1 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[1]);
}
