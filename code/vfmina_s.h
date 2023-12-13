for (int i = 0; i < 2; i++) {
  dst.fp32[i] = (abs(a.fp32[i]) < abs(b.fp32[i])) ? a.fp32[i] : b.fp32[i];
}
