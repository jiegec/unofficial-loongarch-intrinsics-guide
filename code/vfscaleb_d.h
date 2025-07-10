for (int i = 0; i < 2; i++) {
  dst.fp64[i] = __builtin_scalbn(a.fp64[i], b.dword[i]);
}
