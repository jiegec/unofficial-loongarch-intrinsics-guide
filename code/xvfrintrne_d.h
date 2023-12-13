for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (fp64)(s64)a.fp64[i]; // rounding mode is not expressed in C
}
