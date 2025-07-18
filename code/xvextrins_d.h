int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (i == ((imm >> 4) & 1)) ? b.dword[imm & 1] : a.dword[i];
}
for (; i < 4; i++) {
  dst.dword[i] =
      (i - 2 == ((imm >> 4) & 1)) ? b.dword[(imm & 1) + 2] : a.dword[i];
}

// Expands to:

if (0) {
  dst.dword[0] = (0 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[0]);
  dst.dword[1] = (1 == ((imm >> 4) & 1)) ? (b.dword[imm & 1]) : (a.dword[1]);
  dst.dword[2] =
      (0 == ((imm >> 4) & 1)) ? (b.dword[(imm & 1) + 2]) : (a.dword[2]);
  dst.dword[3] =
      (1 == ((imm >> 4) & 1)) ? (b.dword[(imm & 1) + 2]) : (a.dword[3]);
}
