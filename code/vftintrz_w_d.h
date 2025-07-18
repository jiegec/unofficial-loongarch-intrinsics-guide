for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2)
                    ? (s32)b.fp64[i]
                    : (s32)a.fp64[i - 2]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
}
