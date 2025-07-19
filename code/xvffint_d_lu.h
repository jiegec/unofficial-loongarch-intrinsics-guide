for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(u64)a.dword[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp64[0] = (f64)((u64)a.dword[0]);
  dst.fp64[1] = (f64)((u64)a.dword[1]);
  dst.fp64[2] = (f64)((u64)a.dword[2]);
  dst.fp64[3] = (f64)((u64)a.dword[3]);
}
