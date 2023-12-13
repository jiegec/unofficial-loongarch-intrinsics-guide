for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.fp64[i]; // rounding mode is not expressed in C
}
