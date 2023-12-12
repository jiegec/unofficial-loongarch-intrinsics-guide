for (int i = 0; i < 2; i++) {
  dst.dword[i] = ((s64)a.dword[i]) >> (b.dword[i] & 0x3f);
}
