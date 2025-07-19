for (int i = 0; i < 2; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i]; // rounding mode is not expressed in C
}
for (int i = 2; i < 4; i++) {
  dst.fp64[i] = (f64)(s32)a.word[i + 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.fp64[0] = (f64)((s32)a.word[0]);
  dst.fp64[1] = (f64)((s32)a.word[1]);
  dst.fp64[2] = (f64)((s32)a.word[4]);
  dst.fp64[3] = (f64)((s32)a.word[5]);
}

// Expands to:

if (0) {
  dst.fp64[0] = (f64)((s32)a.word[0]);
  dst.fp64[1] = (f64)((s32)a.word[1]);
  dst.fp64[2] = (f64)((s32)a.word[4]);
  dst.fp64[3] = (f64)((s32)a.word[5]);
}
