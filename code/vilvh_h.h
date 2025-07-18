for (int i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2 + 4] : b.half[i / 2 + 4];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[4];
  dst.half[1] = a.half[4];
  dst.half[2] = b.half[5];
  dst.half[3] = a.half[5];
  dst.half[4] = b.half[6];
  dst.half[5] = a.half[6];
  dst.half[6] = b.half[7];
  dst.half[7] = a.half[7];
}
