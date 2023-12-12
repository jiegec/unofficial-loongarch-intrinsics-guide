for (int i = 0; i < 2; i++) {
  dst.dword[i] = (c.dword[i] & b.dword[i]) | (~c.dword[i] & a.dword[i]);
}