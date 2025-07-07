for (int i = 0; i < 4; i++) {
  // dst[0-3] <- b[0-3]
  dst.fp16[i] = b.fp32[i];
}
for (int i = 4; i < 8; i++) {
  // dst[4-7] <- a[0-3]
  dst.fp16[i] = a.fp32[i - 4];
}
for (int i = 8; i < 12; i++) {
  // dst[8-11] <- b[4-7]
  dst.fp16[i] = b.fp32[i - 4];
}
for (int i = 12; i < 16; i++) {
  // dst[12-15] <- a[4-7]
  dst.fp16[i] = a.fp32[i - 8];
}
