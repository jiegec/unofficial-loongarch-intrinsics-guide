for (int i = 0; i < 4; i++) {
  dst.dword[i] = (u64)a.fp64[i]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (u64)a.fp64[0];
  dst.dword[1] = (u64)a.fp64[1];
  dst.dword[2] = (u64)a.fp64[2];
  dst.dword[3] = (u64)a.fp64[3];
}
