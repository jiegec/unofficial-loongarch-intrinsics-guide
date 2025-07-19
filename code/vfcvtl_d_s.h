for (int i = 0; i < 2; i++) {
  dst.fp64[i] = a.fp32[i];
}

// Expands to:

if (0) {
  dst.fp64[0] = a.fp32[0];
  dst.fp64[1] = a.fp32[1];
}
