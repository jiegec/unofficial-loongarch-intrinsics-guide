for (int i = 0; i < 8; i++) {
  dst.half[i] = (i < 4) ? b.half[i * 2 + 1] : a.half[(i - 4) * 2 + 1];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[1];
  dst.half[1] = b.half[3];
  dst.half[2] = b.half[5];
  dst.half[3] = b.half[7];
  dst.half[4] = a.half[1];
  dst.half[5] = a.half[3];
  dst.half[6] = a.half[5];
  dst.half[7] = a.half[7];
}
