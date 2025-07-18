for (int i = 0; i < 16; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i] : b.half[i + 1];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[1];
  dst.half[1] = a.half[1];
  dst.half[2] = b.half[3];
  dst.half[3] = a.half[3];
  dst.half[4] = b.half[5];
  dst.half[5] = a.half[5];
  dst.half[6] = b.half[7];
  dst.half[7] = a.half[7];
  dst.half[8] = b.half[9];
  dst.half[9] = a.half[9];
  dst.half[10] = b.half[11];
  dst.half[11] = a.half[11];
  dst.half[12] = b.half[13];
  dst.half[13] = a.half[13];
  dst.half[14] = b.half[15];
  dst.half[15] = a.half[15];
}
