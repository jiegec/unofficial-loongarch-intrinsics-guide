for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i + 2]; // rounding mode is not expressed in C
}
