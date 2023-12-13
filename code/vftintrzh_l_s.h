for (int i = 0; i < 2; i++) {
  dst.dword[i] = a.fp32[i + 2]; // rounding mode is not expressed in C
}
