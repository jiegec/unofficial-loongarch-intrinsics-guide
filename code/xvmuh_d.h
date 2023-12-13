for (int i = 0; i < 4; i++) {
  dst.dword[i] = (((s128)(s64)a.dword[i] * (s128)(s64)b.dword[i])) >> 64;
}
