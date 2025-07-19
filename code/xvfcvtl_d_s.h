for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i];
}
for (; i < 4; i++) {
  dst.fp64[i] = a.fp32[i + 2];
}

// Expands to:

if (0) {
  dst.fp64[0] = a.fp32[0];
  dst.fp64[1] = a.fp32[1];
  dst.fp64[2] = a.fp32[4];
  dst.fp64[3] = a.fp32[5];
}
