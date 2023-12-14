for (int i = 0; i < 16; i++) {
  if (i < 8) {
    dst.fp16[i] = b.fp32[i];
  } else {
    dst.fp16[i] = a.fp32[i - 8];
  }
}