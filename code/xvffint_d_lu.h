for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(u64)a.dword[i]; // rounding mode is not expressed in C
}
