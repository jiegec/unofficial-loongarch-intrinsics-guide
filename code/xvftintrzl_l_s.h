for (int i = 0; i < 4; i++) {
  dst.dword[i] = (s32)a.fp32[i]; // rounding mode is not expressed in C
}
