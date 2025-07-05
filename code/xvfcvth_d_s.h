for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i + 2];
}
for (int i = 2; i < 4; i++) {
  dst.fp64[i] = a.fp32[i + 4];
}
