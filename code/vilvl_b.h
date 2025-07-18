for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i / 2] : b.byte[i / 2];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = a.byte[0];
  dst.byte[2] = b.byte[1];
  dst.byte[3] = a.byte[1];
  dst.byte[4] = b.byte[2];
  dst.byte[5] = a.byte[2];
  dst.byte[6] = b.byte[3];
  dst.byte[7] = a.byte[3];
  dst.byte[8] = b.byte[4];
  dst.byte[9] = a.byte[4];
  dst.byte[10] = b.byte[5];
  dst.byte[11] = a.byte[5];
  dst.byte[12] = b.byte[6];
  dst.byte[13] = a.byte[6];
  dst.byte[14] = b.byte[7];
  dst.byte[15] = a.byte[7];
}
