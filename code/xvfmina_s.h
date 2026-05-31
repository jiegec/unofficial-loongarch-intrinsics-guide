for (int i = 0; i < 8; i++) {
  if (abs(a.fp32[i]) < abs(b.fp32[i])) {
    dst.fp32[i] = a.fp32[i];
  } else if (abs(b.fp32[i]) < abs(a.fp32[i])) {
    dst.fp32[i] = b.fp32[i];
  } else {
    dst.fp32[i] = fmin(a.fp32[i], b.fp32[i]);
  }
}
