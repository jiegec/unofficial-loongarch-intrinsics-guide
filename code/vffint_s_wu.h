for (int i = 0; i < 4; i++) {
  dst.fp32[i] = (f32)(u32)a.word[i]; // rounding mode is not expressed in C
}
