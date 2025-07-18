for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2)
                    ? (s32)b.fp64[i]
                    : (s32)a.fp64[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6)
                    ? (s32)b.fp64[i - 2]
                    : (s32)a.fp64[i - 4]; // rounding mode is not expressed in C
}

// Expands to:

if (0) {
  dst.word[0] = (s32)b.fp64[0];
  dst.word[1] = (s32)b.fp64[1];
  dst.word[2] = (s32)a.fp64[0];
  dst.word[3] = (s32)a.fp64[1];
  dst.word[4] = (s32)b.fp64[2];
  dst.word[5] = (s32)b.fp64[3];
  dst.word[6] = (s32)a.fp64[2];
  dst.word[7] = (s32)a.fp64[3];
}
