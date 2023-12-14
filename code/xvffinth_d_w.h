for (int i = 0; i < 4; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i + 4]; // rounding mode is not expressed in C
}
