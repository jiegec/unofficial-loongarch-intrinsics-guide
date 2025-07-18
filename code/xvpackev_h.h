for (int i = 0; i < 16; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i - 1] : b.half[i];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = a.half[0];
  dst.half[2] = b.half[2];
  dst.half[3] = a.half[2];
  dst.half[4] = b.half[4];
  dst.half[5] = a.half[4];
  dst.half[6] = b.half[6];
  dst.half[7] = a.half[6];
  dst.half[8] = b.half[8];
  dst.half[9] = a.half[8];
  dst.half[10] = b.half[10];
  dst.half[11] = a.half[10];
  dst.half[12] = b.half[12];
  dst.half[13] = a.half[12];
  dst.half[14] = b.half[14];
  dst.half[15] = a.half[14];
}
