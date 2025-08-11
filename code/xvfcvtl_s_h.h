int i;
for (i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[i];
}
for (; i < 8; i++) {
  dst.fp32[i] = a.fp16[i + 4];
}

// Expands to:

if (0) {
  dst.fp32[0] = a.fp16[0];
  dst.fp32[1] = a.fp16[1];
  dst.fp32[2] = a.fp16[2];
  dst.fp32[3] = a.fp16[3];
  dst.fp32[4] = a.fp16[8];
  dst.fp32[5] = a.fp16[9];
  dst.fp32[6] = a.fp16[10];
  dst.fp32[7] = a.fp16[11];
}
