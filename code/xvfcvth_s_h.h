for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[i + 4];
}
for (; i < 8; i++) {
  dst.fp32[i] = a.fp16[i + 8];
}

// Expands to:

if (0) {
  dst.fp32[0] = a.fp16[4];
  dst.fp32[1] = a.fp16[5];
  dst.fp32[2] = a.fp16[6];
  dst.fp32[3] = a.fp16[7];
  dst.fp32[4] = a.fp16[12];
  dst.fp32[5] = a.fp16[13];
  dst.fp32[6] = a.fp16[14];
  dst.fp32[7] = a.fp16[15];
}
