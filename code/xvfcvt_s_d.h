for (int i = 0; i < 2; i++) {
  // dst[0,1] <- b[0,1]
  dst.fp32[i] = b.fp64[i];
}
for (int i = 2; i < 4; i++) {
  // dst[2,3] <- a[0,1]
  dst.fp32[i] = a.fp64[i - 2];
}
for (int i = 4; i < 6; i++) {
  // dst[4,5] <- b[2,3]
  dst.fp32[i] = b.fp64[i - 2];
}
for (int i = 6; i < 8; i++) {
  // dst[6,7] <- a[2,3]
  dst.fp32[i] = a.fp64[i - 4];
}
