for (int i = 0; i < 4; i++) {
  dst.fp32[i] = a.fp16[i + 4];
}
for (int i = 4; i < 8; i++) {
  dst.fp32[i] = a.fp16[i + 8];
}
