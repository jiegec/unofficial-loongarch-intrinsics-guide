for (int i = 0; i < 8; i++) {
  if (i < 4) {
    dst.fp16[i] = b.fp32[i];
  } else {
    dst.fp16[i] = a.fp32[i - 4];
  }
}