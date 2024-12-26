for (int i = 0; i < 4; i++) {
  dst.word[i] = (i < 2)
                    ? (s64)b.fp64[i]
                    : (s64)a.fp64[i - 2]; // rounding mode is not expressed in C
}
for (int i = 4; i < 8; i++) {
  dst.word[i] = (i < 6)
                    ? (s64)b.fp64[i - 2]
                    : (s64)a.fp64[i - 4]; // rounding mode is not expressed in C
}
