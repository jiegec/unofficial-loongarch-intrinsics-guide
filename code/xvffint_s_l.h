for (int i = 0; i < 8; i++) {
  dst.fp32[i] =
      (i < 4) ? (f32)(s32)a.dword[i]
              : (f32)(s32)b.dword[i]; // rounding mode is not expressed in C
}
