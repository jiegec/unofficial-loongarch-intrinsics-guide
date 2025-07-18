for (int i = 0; i < 32; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i - 1] : b.byte[i];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[0];
  dst.byte[1] = a.byte[0];
  dst.byte[2] = b.byte[2];
  dst.byte[3] = a.byte[2];
  dst.byte[4] = b.byte[4];
  dst.byte[5] = a.byte[4];
  dst.byte[6] = b.byte[6];
  dst.byte[7] = a.byte[6];
  dst.byte[8] = b.byte[8];
  dst.byte[9] = a.byte[8];
  dst.byte[10] = b.byte[10];
  dst.byte[11] = a.byte[10];
  dst.byte[12] = b.byte[12];
  dst.byte[13] = a.byte[12];
  dst.byte[14] = b.byte[14];
  dst.byte[15] = a.byte[14];
  dst.byte[16] = b.byte[16];
  dst.byte[17] = a.byte[16];
  dst.byte[18] = b.byte[18];
  dst.byte[19] = a.byte[18];
  dst.byte[20] = b.byte[20];
  dst.byte[21] = a.byte[20];
  dst.byte[22] = b.byte[22];
  dst.byte[23] = a.byte[22];
  dst.byte[24] = b.byte[24];
  dst.byte[25] = a.byte[24];
  dst.byte[26] = b.byte[26];
  dst.byte[27] = a.byte[26];
  dst.byte[28] = b.byte[28];
  dst.byte[29] = a.byte[28];
  dst.byte[30] = b.byte[30];
  dst.byte[31] = a.byte[30];
}
