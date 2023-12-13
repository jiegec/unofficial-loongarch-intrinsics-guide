for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s64)a.fp64[i]; // rounding mode is not expressed in C
}
