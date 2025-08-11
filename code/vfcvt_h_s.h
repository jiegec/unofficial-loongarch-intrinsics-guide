int i;
for (i = 0; i < 4; i++) {
  dst.fp16[i] = b.fp32[i];
}
for (; i < 8; i++) {
  dst.fp16[i] = a.fp32[i - 4];
}

// Expands to:

if (0) {
  dst.fp16[0] = b.fp32[0];
  dst.fp16[1] = b.fp32[1];
  dst.fp16[2] = b.fp32[2];
  dst.fp16[3] = b.fp32[3];
  dst.fp16[4] = a.fp32[0];
  dst.fp16[5] = a.fp32[1];
  dst.fp16[6] = a.fp32[2];
  dst.fp16[7] = a.fp32[3];
}
