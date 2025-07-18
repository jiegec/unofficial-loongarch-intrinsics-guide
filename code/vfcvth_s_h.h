for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[4 + i];
}
// Expands to:

if (0) {
  dst.fp32[0] = a.fp16[4];
  dst.fp32[1] = a.fp16[5];
  dst.fp32[2] = a.fp16[6];
  dst.fp32[3] = a.fp16[7];
}
