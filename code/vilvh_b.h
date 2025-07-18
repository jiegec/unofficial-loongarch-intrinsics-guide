for (int i = 0; i < 16; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i / 2 + 8] : b.byte[i / 2 + 8];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[8];
  dst.byte[1] = a.byte[8];
  dst.byte[2] = b.byte[9];
  dst.byte[3] = a.byte[9];
  dst.byte[4] = b.byte[10];
  dst.byte[5] = a.byte[10];
  dst.byte[6] = b.byte[11];
  dst.byte[7] = a.byte[11];
  dst.byte[8] = b.byte[12];
  dst.byte[9] = a.byte[12];
  dst.byte[10] = b.byte[13];
  dst.byte[11] = a.byte[13];
  dst.byte[12] = b.byte[14];
  dst.byte[13] = a.byte[14];
  dst.byte[14] = b.byte[15];
  dst.byte[15] = a.byte[15];
}
