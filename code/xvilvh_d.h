int i;
for (i = 0; i < 2; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i / 2 + 1] : b.dword[i / 2 + 1];
}
for (; i < 4; i++) {
  dst.dword[i] = (i % 2 == 1) ? a.dword[i / 2 + 2] : b.dword[i / 2 + 2];
}

// Expands to:

if (0) {
  dst.dword[0] = b.dword[1];
  dst.dword[1] = a.dword[1];
  dst.dword[2] = b.dword[3];
  dst.dword[3] = a.dword[3];
}
