for (int i = 0; i < 4; i++) {
  dst.fp32[i] =
      (i < 2) ? (f32)(s64)b.dword[i]
              : (f32)(s64)a.dword[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.fp32[i] =
      (i < 6) ? (f32)(s64)b.dword[i - 2]
              : (f32)(s64)a.dword[i - 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp32[0] = (f32)(s64)b.dword[0];
  dst.fp32[1] = (f32)(s64)b.dword[1];
  dst.fp32[2] = (f32)(s64)a.dword[0];
  dst.fp32[3] = (f32)(s64)a.dword[1];
  dst.fp32[4] = (f32)(s64)b.dword[2];
  dst.fp32[5] = (f32)(s64)b.dword[3];
  dst.fp32[6] = (f32)(s64)a.dword[2];
  dst.fp32[7] = (f32)(s64)a.dword[3];
}
