for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i + 2];
}
for (; i < 4; i++) {
  dst.fp64[i] = a.fp32[i + 4];
}

// Expands to:

if (0) {
  dst.fp64[0] = a.fp32[2];
  dst.fp64[1] = a.fp32[3];
  dst.fp64[2] = a.fp32[6];
  dst.fp64[3] = a.fp32[7];
}
