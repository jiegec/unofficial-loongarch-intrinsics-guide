for (int i = 0; i < 2; i++) {
  dst.dword[i] = (s64)a.fp32[i + 2]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.dword[i] = (s64)a.fp32[i + 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.dword[0] = (s64)a.fp32[2];
  dst.dword[1] = (s64)a.fp32[3];
  dst.dword[2] = (s64)a.fp32[6];
  dst.dword[3] = (s64)a.fp32[7];
}
