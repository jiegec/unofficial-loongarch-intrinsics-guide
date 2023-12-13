for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (fp32)(s32)a.fp32[i]; // rounding mode is not expressed in C
}
