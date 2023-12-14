for (int i = 0; i < 8; i++) {
  if (i < 4) {
    dst.fp32[i] = b.fp64[i];
  } else {
    dst.fp32[i] = a.fp64[i - 4];
  }
}