for (int i = 0; i < 32; i++) {
  dst.byte[i] = (i % 2 == 1) ? a.byte[i] : b.byte[i + 1];
}

// Expands to:

if (0) {
  dst.byte[0] = b.byte[1];
  dst.byte[1] = a.byte[1];
  dst.byte[2] = b.byte[3];
  dst.byte[3] = a.byte[3];
  dst.byte[4] = b.byte[5];
  dst.byte[5] = a.byte[5];
  dst.byte[6] = b.byte[7];
  dst.byte[7] = a.byte[7];
  dst.byte[8] = b.byte[9];
  dst.byte[9] = a.byte[9];
  dst.byte[10] = b.byte[11];
  dst.byte[11] = a.byte[11];
  dst.byte[12] = b.byte[13];
  dst.byte[13] = a.byte[13];
  dst.byte[14] = b.byte[15];
  dst.byte[15] = a.byte[15];
  dst.byte[16] = b.byte[17];
  dst.byte[17] = a.byte[17];
  dst.byte[18] = b.byte[19];
  dst.byte[19] = a.byte[19];
  dst.byte[20] = b.byte[21];
  dst.byte[21] = a.byte[21];
  dst.byte[22] = b.byte[23];
  dst.byte[23] = a.byte[23];
  dst.byte[24] = b.byte[25];
  dst.byte[25] = a.byte[25];
  dst.byte[26] = b.byte[27];
  dst.byte[27] = a.byte[27];
  dst.byte[28] = b.byte[29];
  dst.byte[29] = a.byte[29];
  dst.byte[30] = b.byte[31];
  dst.byte[31] = a.byte[31];
}
