for (int i = 0; i < 2; i++) {
  dst.dword[i] = (i < 1)
                     ? (s64)a.fp64[i]
                     : (s64)b.fp64[i]; // rounding mode is not expressed in C
}
