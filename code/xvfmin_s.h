for (int i = 0; i < 8; i++) {
  dst.fp32[i] = fmin(a.fp32[i], b.fp32[i]);
}
