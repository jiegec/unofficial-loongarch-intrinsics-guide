int i;
for (i = 0; i < 4; i++) {
  dst.fp16[i] = b.fp32[i];
}
for (; i < 8; i++) {
  dst.fp16[i] = a.fp32[i - 4];
}
for (; i < 12; i++) {
  dst.fp16[i] = b.fp32[i - 4];
}
for (; i < 16; i++) {
  dst.fp16[i] = a.fp32[i - 8];
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
  dst.fp16[8] = b.fp32[4];
  dst.fp16[9] = b.fp32[5];
  dst.fp16[10] = b.fp32[6];
  dst.fp16[11] = b.fp32[7];
  dst.fp16[12] = a.fp32[4];
  dst.fp16[13] = a.fp32[5];
  dst.fp16[14] = a.fp32[6];
  dst.fp16[15] = a.fp32[7];
}
