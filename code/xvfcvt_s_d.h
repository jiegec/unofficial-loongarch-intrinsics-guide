for (int i = 0; i < 2; i++) {
  dst.fp32[i] = b.fp64[i];
}
for (; i < 4; i++) {
  dst.fp32[i] = a.fp64[i - 2];
}
for (; i < 6; i++) {
  dst.fp32[i] = b.fp64[i - 2];
}
for (; i < 8; i++) {
  dst.fp32[i] = a.fp64[i - 4];
}

// Expands to:

if (0) {
  dst.fp32[0] = b.fp64[0];
  dst.fp32[1] = b.fp64[1];
  dst.fp32[2] = a.fp64[0];
  dst.fp32[3] = a.fp64[1];
  dst.fp32[4] = b.fp64[2];
  dst.fp32[5] = b.fp64[3];
  dst.fp32[6] = a.fp64[2];
  dst.fp32[7] = a.fp64[3];
}
