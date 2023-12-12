for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i] >> 1) + ((s64)b.dword[i] >> 1) +
                 ((a.dword[i] | b.dword[i]) & 1);
}
