for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
