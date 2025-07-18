for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? b.half[i * 2] : a.half[(i - 4) * 2];
}
for (int i = 8; i < 16; i++) {
  dst.half[i] = (i < 12) ? b.half[(i - 4) * 2] : a.half[(i - 8) * 2];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = b.half[2];
  dst.half[2] = b.half[4];
  dst.half[3] = b.half[6];
  dst.half[4] = a.half[0];
  dst.half[5] = a.half[2];
  dst.half[6] = a.half[4];
  dst.half[7] = a.half[6];
  dst.half[8] = b.half[8];
  dst.half[9] = b.half[10];
  dst.half[10] = b.half[12];
  dst.half[11] = b.half[14];
  dst.half[12] = a.half[8];
  dst.half[13] = a.half[10];
  dst.half[14] = a.half[12];
  dst.half[15] = a.half[14];
}
