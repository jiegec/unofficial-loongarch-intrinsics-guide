int i;
for (i = 0; i < 8; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2] : b.half[i / 2];
}
for (; i < 16; i++) {
  dst.half[i] = (i % 2 == 1) ? a.half[i / 2 + 4] : b.half[i / 2 + 4];
}

// Expands to:

if (0) {
  dst.half[0] = b.half[0];
  dst.half[1] = a.half[0];
  dst.half[2] = b.half[1];
  dst.half[3] = a.half[1];
  dst.half[4] = b.half[2];
  dst.half[5] = a.half[2];
  dst.half[6] = b.half[3];
  dst.half[7] = a.half[3];
  dst.half[8] = b.half[8];
  dst.half[9] = a.half[8];
  dst.half[10] = b.half[9];
  dst.half[11] = a.half[9];
  dst.half[12] = b.half[10];
  dst.half[13] = a.half[10];
  dst.half[14] = b.half[11];
  dst.half[15] = a.half[11];
}
